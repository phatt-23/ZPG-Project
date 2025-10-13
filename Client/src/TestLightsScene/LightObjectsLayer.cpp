#include "LightObjectsLayer.h"
#include "Models/box.h"

using namespace ZPG;

void LightObjectsLayer::OnAttach() {
    auto VBO = ZPG::VertexBuffer::Create(BoxModel::boxVertices, sizeof(BoxModel::boxVertices));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
        {ZPG::ShaderDataType::Float2, "a_TexCoord"},
    });

    auto IBO = ZPG::IndexBuffer::Create(BoxModel::boxIndices, ZPG_ARRAY_LENGTH(BoxModel::boxIndices));
    
    m_VAO = ZPG::VertexArray::Create();
    m_VAO->AddVertexBuffer(VBO);
    m_VAO->SetIndexBuffer(IBO);
}

void LightObjectsLayer::OnRender(const ZPG::RenderContext& ctx) {
    for (auto& light : ctx.m_Lights) {
        if (light->GetLightType() == LightType::Point) {
            PointLight* pointLight = (PointLight*)light.get();
            Renderer::Submit(
                *m_Scene->GetResourceManager().GetShaderProgram("basic_single_color"),
                *m_VAO, 
                Translate(pointLight->GetPosition()).GetMatrix()
            );
        }
    }
}
