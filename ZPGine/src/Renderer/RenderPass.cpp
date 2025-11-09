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
#include "Texture/Texture.h"

namespace ZPG
{

    void RenderPass::Flush(RenderContext& renderContext)
    {
        ZPG_PROFILE_FUNCTION();
        if (renderContext.Batch.GetBatchSize() == 0) {
            return;
        }

        renderContext.Batch.BuildGroups();

        const auto& materialGroups = renderContext.Batch.GetMaterialGroups();
        const auto& vaoGroups = renderContext.Batch.GetVertexArrayGroups();
        const auto& modelTransforms = renderContext.Batch.GetTransforms();
        const auto& entityIDs = renderContext.Batch.GetEntityIDs();

        for (const auto& materialGroup : materialGroups)
        {
            auto material = materialGroup.m_Material;

            material->GetAlbedoMap()->BindToSlot(RenderBindingPoints::ALBEDO_MAP);
            material->GetMetalnessMap()->BindToSlot(RenderBindingPoints::METALNESS_MAP);
            material->GetRoughnessMap()->BindToSlot(RenderBindingPoints::ROUGHNESS_MAP);
            material->GetNormalMap()->BindToSlot(RenderBindingPoints::NORMAL_MAP);
            material->GetEmissiveMap()->BindToSlot(RenderBindingPoints::EMISSIVE_MAP);

            renderContext.MaterialSSBO.SetMaterial(*material);

            for (int vaoIdx = materialGroup.m_VertexArrayStart;
                vaoIdx < materialGroup.m_VertexArrayStart + materialGroup.m_VertexArrayCount;
                vaoIdx++)
            {
                auto vaoGroup = vaoGroups[vaoIdx];
                auto vao = vaoGroup.m_VertexArray;

                vao->Bind();

                renderContext.ModelSSBO.SetModels(&modelTransforms[vaoGroup.m_Start], vaoGroup.m_Count);
                renderContext.EntitySSBO.SetEntityIDs(&entityIDs[vaoGroup.m_Start], vaoGroup.m_Count);

                if (vaoGroup.m_VertexArray->HasIndexBuffer()) {
                    RenderCommand::DrawIndexedInstanced(*vao, vao->GetIndexBuffer()->GetCount(), vaoGroup.m_Count);
                }
                else {
                    RenderCommand::DrawArraysInstanced(*vao, vaoGroup.m_Count);
                }

                vao->Unbind();

                renderContext.Statistics.DrawCallCountPerFrame++;
            }

        }

        renderContext.Batch.Reset();

        renderContext.Statistics.FlushCountPerFrame++;
    }
}
