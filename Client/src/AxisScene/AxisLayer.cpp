#include "AxisLayer.h"
#include "Entity/Entity.h"
#include "ZPGine.h"
#include "../assets/models/phatt/box.h"

using namespace ZPG;

namespace AxisSceneNS {

void AxisLayer::OnAttach() {
    m_ShaderProgram = ShaderProgram::Create("AxisShaderProgram", {
        Shader::Create("./assets/shaders/vertex/DefaultLit.vert"),
        Shader::CreateFromCode("just_a_single_color_thing", Shader::Fragment, R"(
            #version 330 core
            uniform vec4 u_Albedo;
            out vec4 f_FragColor;
            void main() {
                f_FragColor = u_Albedo;
            }
        )"),
    });

    auto VAO = VertexArray::Create({
        VertexBuffer::Create(
            phatt::boxVertices,
            sizeof(phatt::boxVertices),
            {
                {ShaderDataType::Float3, "a_Pos"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float2, "a_UV"},
            }
        ),
    }, IndexBuffer::Create(phatt::boxIndices, ZPG_ARRAY_LENGTH(phatt::boxIndices)));

    f32 length = 200.0f;
    auto x = MakeRef<Mesh>(VAO, Scale(glm::vec3(length, 0.1, 0.1)).GetMatrix());
    auto y = MakeRef<Mesh>(VAO, Scale(glm::vec3(0.1, length, 0.1)).GetMatrix());
    auto z = MakeRef<Mesh>(VAO, Scale(glm::vec3(0.1, 0.1, length)).GetMatrix());

    auto xm = MakeRef(new Material());
    xm->SetShaderProgram(m_ShaderProgram);
    xm->SetAlbedo({1.0, 0.0, 0.0, 1.0});
    x->SetMaterial(xm);

    auto ym = MakeRef(new Material());
    ym->SetShaderProgram(m_ShaderProgram);
    ym->SetAlbedo({0.0, 1.0, 0.0, 1.0});
    y->SetMaterial(ym);

    auto zm = MakeRef(new Material());
    zm->SetShaderProgram(m_ShaderProgram);
    zm->SetAlbedo({0.0, 0.0, 1.0, 1.0});
    z->SetMaterial(zm);

    auto model = MakeRef<Model>(new Model({ x, y, z }));

    auto transform = MakeRef<Translate>(glm::vec3(0.0f));

    m_Entity = MakeRef(new Entity(model, transform));
}

void AxisLayer::OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) {
    Renderer::SubmitEntity(*m_Entity);
}

}
