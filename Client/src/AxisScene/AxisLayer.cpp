#include "AxisLayer.h"

namespace AxisSceneNS {

AxisLayer::AxisLayer() {}

AxisLayer::~AxisLayer() {}

void AxisLayer::OnAttach() {
    using namespace ZPG;

    m_ShaderProgram = ShaderProgram::Create("AxisShaderProgram", {
        Shader::Create("./assets/shaders/vertex/DefaultLit.vert"),
        Shader::CreateFromCode("just_a_single_color_thing", Shader::Fragment, R"(
            #version 330 core
            out vec4 f_FragColor;
            void main() {
                f_FragColor = vec4(1.0, 0.1, 0.1, 0.5);
            }
            )"),
    });

    auto VAO = VertexArray::Create({
        VertexBuffer::Create(
            BoxModel::boxVertices, 
            sizeof(BoxModel::boxVertices),
            {
                {ShaderDataType::Float3, "a_Pos"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float2, "a_UV"},
            }
        ),
    }, IndexBuffer::Create(BoxModel::boxIndices, ZPG_ARRAY_LENGTH(BoxModel::boxIndices)));

    m_Model = CreateScope<Model>(new Model({
        CreateRef<Mesh>(VAO, ScaleTransform(glm::vec3(10.0, 0.1, 0.1)).GetMatrix()),
        CreateRef<Mesh>(VAO, ScaleTransform(glm::vec3(0.1, 10.0, 0.1)).GetMatrix()),
        CreateRef<Mesh>(VAO, ScaleTransform(glm::vec3(0.1, 0.1, 10.0)).GetMatrix()),
    }));
}

void AxisLayer::OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) {
    using namespace ZPG;
    Renderer::BeginDraw(ctx.m_Camera);
        Renderer::Submit(*m_ShaderProgram, *m_Model);
    Renderer::EndDraw();
}

}
