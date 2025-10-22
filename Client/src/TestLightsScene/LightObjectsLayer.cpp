#include "LightObjectsLayer.h"
#include "../assets/models/phatt/box.h"

using namespace ZPG;

void LightObjectsLayer::OnAttach() {
    auto VBO = ZPG::VertexBuffer::Create(phatt::boxVertices, sizeof(phatt::boxVertices));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
        {ZPG::ShaderDataType::Float2, "a_TexCoord"},
    });

    auto IBO = ZPG::IndexBuffer::Create(phatt::boxIndices, ZPG_ARRAY_LENGTH(phatt::boxIndices));
    
    m_VAO = ZPG::VertexArray::Create();
    m_VAO->AddVertexBuffer(VBO);
    m_VAO->SetIndexBuffer(IBO);
}

void LightObjectsLayer::OnRender(const ZPG::RenderContext& ctx) {
    for (auto& light : ctx.Lights) {
        if (light->GetLightType() == LightType::Point) {
            PointLight* pointLight = (PointLight*)light.get();
            Renderer::Submit(
                *m_Scene->GetResourceManager().GetShaderProgram("basic_single_color"),
                *m_VAO, 
                Translate(pointLight->m_Position.GetPosition()).GetMatrix()
            );
        }
    }
}
