#include "TestTextureScene.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

using namespace ZPG;

namespace TestTextureSceneNS {


f32 random_f32(f32 min, f32 max) {
    return (max - min) * (f32)rand() / (f32)RAND_MAX + min;
}

f32 boxVertices[] = {
    -0.1f, -0.1f,  0.1f,    1.0, 0.0, 0.0, 1.0,  1.0, 0.0, // L D F
    -0.1f, -0.1f, -0.1f,    0.0, 1.0, 0.0, 1.0,  0.0, 0.0, // L D B
    0.1f, -0.1f, -0.1f,    0.0, 0.0, 1.0, 1.0,   0.0, 1.0, // R D B
    0.1f, -0.1f,  0.1f,    1.0, 0.0, 0.0, 1.0,   1.0, 1.0, // R D F
    -0.1f,  0.1f,  0.1f,    0.0, 1.0, 0.0, 1.0,  1.0, 0.0, // L U F
    -0.1f,  0.1f, -0.1f,    0.0, 0.0, 1.0, 1.0,  0.0, 0.0, // L U B
    0.1f,  0.1f, -0.1f,    1.0, 1.0, 1.0, 1.0,   0.0, 1.0, // R U B
    0.1f,  0.1f,  0.1f,    0.0, 1.0, 1.0, 1.0,   1.0, 1.0, // R U F
};

u32 boxIndices[] = {
    0, 1, 2,
    0, 2, 3,
    0, 4, 7,
    0, 7, 3,
    3, 7, 6,
    3, 6, 2,
    2, 6, 5,
    2, 5, 1,
    1, 5, 4,
    1, 4, 0,
    4, 5, 6,
    4, 6, 7,
};


f32 planeVertices[] = {
    -0.1f, -0.5f, 0.1f,     1.0, 1.0, 1.0, 1.0,   0.0, 0.0, // front left
     0.1f, -0.5f, 0.1f,     1.0, 1.0, 1.0, 1.0,   1.0, 0.0, // front right
     0.1f, -0.5f, -0.1f,    1.0, 1.0, 1.0, 1.0,   1.0, 1.0, // back right
    -0.1f, -0.5f, -0.1f,    1.0, 1.0, 1.0, 1.0,   0.0, 1.0, // back left
};

u32 planeIndices[] = {
    0, 1, 2,
    0, 2, 3,
};


class MyLayer : public Layer {
    Ref<VertexArray> m_BoxVao;
    Ref<VertexArray> m_PlaneVao;
    Ref<ShaderProgram> m_ShaderProgram;
    Timestep m_Timestep = 0;
    std::vector<glm::mat4> m_Transforms;
    Ref<Texture> m_Texture;
public:
    MyLayer() {
    }
    void OnAttach() override {
        m_Texture = Texture::Create("./assets/textures/night_bridge_phone.png");

        auto vertexShader = Shader::Create("./assets/shaders/vertex/basic_lit.vert");
        auto fragmentShader = Shader::Create("./assets/shaders/fragment/basic_one_uniform_texture.frag");
        
        m_ShaderProgram = ShaderProgram::Create(
            "testing_texture", { vertexShader, fragmentShader });

        auto boxVbo = VertexBuffer::Create(boxVertices, sizeof(boxVertices));
        boxVbo->SetLayout({
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });

        auto boxIbo = IndexBuffer::Create(boxIndices, ZPG_ARRAY_LENGTH(boxIndices));

        m_BoxVao = VertexArray::Create();
        m_BoxVao->AddVertexBuffer(boxVbo);
        m_BoxVao->SetIndexBuffer(boxIbo);
        
        auto planeVbo = VertexBuffer::Create(planeVertices, sizeof(planeVertices));
        planeVbo->SetLayout({
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float4, "a_Color"},
            {ShaderDataType::Float2, "a_TexCoord"}
        });

        auto planeIbo = IndexBuffer::Create(planeIndices, ZPG_ARRAY_LENGTH(planeIndices));
        m_PlaneVao = VertexArray::Create();
        m_PlaneVao->AddVertexBuffer(planeVbo);
        m_PlaneVao->SetIndexBuffer(planeIbo);

        for (int i = 0; i < 1000; i++)
            m_Transforms.push_back(glm::translate(glm::mat4(1.f), glm::vec3(random_f32(-10.f, 10.f), random_f32(-10.f, 10.f), random_f32(-10.f, 10.f))));
    }
    void OnUpdate(SceneContext& context) override {
        m_Timestep = context.m_Timestep;
    }
    void OnRender(const RenderContext& ctx) override {
        using namespace glm;

        static f32 rot = 0;
        rot += 50.f * ctx.m_Timestep;

        mat4 rotMat = rotate(mat4(1.f), radians(rot), vec3(0.f, 1.f, 0.f));
        mat4 trMat  = translate(mat4(1.f), vec3(1.0, 0.0, 0.0));
        mat4 scMat = scale(mat4(1.f), vec3(2.0, 2.0, 2.0));
        mat4 boxTransform = rotMat * trMat * rotMat * scMat;

        static glm::mat4 planeTransform = glm::scale(glm::mat4(1.f), glm::vec3(10.f, 1.0f, 20.f));

        m_Texture->BindToSlot(2);
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt("u_Texture", 2);

        Renderer::Submit(*m_ShaderProgram, 
                            *m_BoxVao, 
                            boxTransform);
        Renderer::Submit(*m_ShaderProgram, 
                            *m_BoxVao, 
                            scMat * boxTransform);
        Renderer::Submit(*m_ShaderProgram, 
                            *m_BoxVao, 
                            boxTransform * scMat);
        Renderer::Submit(*m_ShaderProgram, 
                            *m_BoxVao, 
                            boxTransform * scMat);
        Renderer::Submit(*m_ShaderProgram, 
                            *m_BoxVao, 
                            scMat * translate(mat4(1.f), vec3(10.0, 0, 0)) * scMat);
        Renderer::Submit(*m_ShaderProgram, *m_PlaneVao, planeTransform);

        for (auto& tr : m_Transforms) {
            Renderer::Submit(*m_ShaderProgram, *m_BoxVao, tr);
        }
    }
};

TestTextureScene::TestTextureScene() 
: m_CameraController(GetCamera()) {
    f32 aspect = Application::Get().GetWindow().GetAspectRatio();
    Camera& c = m_CameraController.GetCamera();
    c.SetPerspectiveProjection(90.f, aspect, c.GetZNear(), c.GetZFar());
}
void TestTextureScene::OnAttach() {
    PushLayer(new MyLayer());
}
void TestTextureScene::OnUpdate(Timestep ts) {
    m_CameraController.OnUpdate(ts);
    UpdateLayers(ts);
}
void TestTextureScene::OnEvent(Event &event) {
    m_CameraController.OnEvent(event);
    PropagateEventDownLayers(event);
}

}
