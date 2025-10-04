#include "LightObjectsLayer.h"
#include "Models/box.h"

using namespace ZPG;

void LightObjectsLayer::OnAttach() {
    m_ShaderProgram = ZPG::ShaderProgram::Create("basic_lighting+phong", {
        Shader::Create("./assets/shaders/vertex/basic_lighting.vert"),
        Shader::Create("./assets/shaders/fragment/single_color.frag"),
    });

    auto VBO = ZPG::VertexBuffer::Create(BoxModel::boxVertices, sizeof(BoxModel::boxVertices)/sizeof(*BoxModel::boxVertices));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
        {ZPG::ShaderDataType::Float2, "a_TexCoord"},
    });

    auto IBO = ZPG::IndexBuffer::Create(BoxModel::boxIndices, sizeof(BoxModel::boxIndices)/sizeof(*BoxModel::boxIndices));
    
    m_VAO = ZPG::VertexArray::Create();
    m_VAO->AddVertexBuffer(VBO);
    m_VAO->SetIndexBuffer(IBO);
}

void LightObjectsLayer::OnRender(const ZPG::RenderContext& ctx) {
    ZPG::Renderer::BeginDraw(ctx.m_Camera);
        for (auto& light : ctx.m_Lights) {
            if (light->GetLightType() == LightType::Point) {
                PointLight* pointLight = (PointLight*)light.get();
                Renderer::Submit(
                    m_ShaderProgram, 
                    m_VAO, 
                    TranslationTransform(pointLight->GetPosition()).GetMatrix()
                );
            }
        }
    ZPG::Renderer::EndDraw();
}
