//
// Created by phatt on 11/8/25.
//

#include "RenderPass.h"

#include "RenderBindingPoints.h"
#include "RenderCommand.h"
#include "RenderContext.h"
#include "RenderGroups.h"
#include "Buffer/VertexArray.h"
#include "Material/Material.h"
#include "Profiling/Instrumentor.h"
#include "Texture/Texture2D.h"

namespace ZPG
{

    void RenderPass::Flush(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();
        if (context.Batch.GetBatchSize() == 0) {
            return;
        }

        context.Batch.BuildGroups();

        const auto& materialGroups = context.Batch.GetMaterialGroups();
        const auto& vaoGroups = context.Batch.GetVertexArrayGroups();
        const auto& modelTransforms = context.Batch.GetTransforms();
        const auto& entityIDs = context.Batch.GetEntityIDs();

        for (const auto& materialGroup : materialGroups)
        {
            auto material = materialGroup.m_Material;

            material->GetAlbedoMap()->BindToSlot(RenderBindingPoints::ALBEDO_MAP);
            material->GetMetalnessMap()->BindToSlot(RenderBindingPoints::METALNESS_MAP);
            material->GetRoughnessMap()->BindToSlot(RenderBindingPoints::ROUGHNESS_MAP);
            material->GetNormalMap()->BindToSlot(RenderBindingPoints::NORMAL_MAP);
            material->GetEmissiveMap()->BindToSlot(RenderBindingPoints::EMISSIVE_MAP);

            context.MaterialSSBO.SetMaterial(*material);

            for (int vaoIdx = materialGroup.m_VertexArrayStart;
                vaoIdx < materialGroup.m_VertexArrayStart + materialGroup.m_VertexArrayCount;
                vaoIdx++)
            {
                auto vaoGroup = vaoGroups[vaoIdx];
                auto vao = vaoGroup.m_VertexArray;

                vao->Bind();

                context.ModelSSBO.SetModels(&modelTransforms[vaoGroup.m_Start], vaoGroup.m_Count);
                context.EntitySSBO.SetEntityIDs(&entityIDs[vaoGroup.m_Start], vaoGroup.m_Count);

                if (vaoGroup.m_VertexArray->HasIndexBuffer()) {
                    RenderCommand::DrawIndexedInstanced(*vao, vao->GetIndexBuffer()->GetCount(), vaoGroup.m_Count);
                }
                else {
                    RenderCommand::DrawArraysInstanced(*vao, vaoGroup.m_Count);
                }

                vao->Unbind();

                context.Statistics.DrawCallCountPerFrame++;
            }

        }

        context.Batch.Reset();

        context.Statistics.FlushCountPerFrame++;
    }
}
