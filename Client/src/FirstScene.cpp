#include "FirstScene.h"
#include <glm/gtc/quaternion.hpp>




class MainLayer : public ZPG::Layer {
    ZPG::Ref<ZPG::VertexArray> vao;
    ZPG::Ref<ZPG::VertexBuffer> vbo;
    ZPG::Ref<ZPG::IndexBuffer> ibo;
    ZPG::Ref<ZPG::ShaderProgram> shaderProgram;
public:
    MainLayer() : m_CameraController() {
        ZPG::f32 aspectRatio = ZPG::Application::Get().GetWindow().GetAspectRatio();
        ZPG::Camera& camera = m_CameraController.GetCamera();
        camera.SetPerspectiveProjection(camera.GetFOV(), aspectRatio, 0.001f, camera.GetZFar());
        camera.SetPosition(glm::vec3(0.0, 0.0, 1.0));
        camera.SetOrientation(glm::quatLookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)));
    }
    void OnAttach() override {
        ZPG_INFO("{}", __PRETTY_FUNCTION__);

        ZPG::f32 vertices[] = {
            -0.1f, -0.1f,  0.1f,    1.0, 0.0, 0.0, 1.0,
            -0.1f, -0.1f, -0.1f,    0.0, 1.0, 0.0, 1.0,
             0.1f, -0.1f, -0.1f,    0.0, 0.0, 1.0, 1.0,
             0.1f, -0.1f,  0.1f,    1.0, 0.0, 0.0, 1.0,
            -0.1f,  0.1f,  0.1f,    0.0, 1.0, 0.0, 1.0,
            -0.1f,  0.1f, -0.1f,    0.0, 0.0, 1.0, 1.0,
             0.1f,  0.1f, -0.1f,    1.0, 1.0, 1.0, 1.0,
             0.1f,  0.1f,  0.1f,    0.0, 1.0, 1.0, 1.0,
        };

        ZPG::u32 indices[] = {
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

        auto vbo = ZPG::VertexBuffer::Create(vertices, sizeof(vertices)/sizeof(*vertices));
        vbo->SetLayout({
            {ZPG::ShaderDataType::Float3, "a_Pos"},
            {ZPG::ShaderDataType::Float4, "a_Color"},
        });

        ibo = ZPG::IndexBuffer::Create(indices, sizeof(indices)/sizeof(*indices));
        vao = ZPG::VertexArray::Create();

        vao->SetIndexBuffer(ibo);
        vao->AddVertexBuffer(vbo);
        
        shaderProgram = ZPG::ShaderProgram::Create("./assets/shaders/basic.glsl");
    }
    void OnUpdate(ZPG::Timestep ts) override {
        static ZPG::f32 rot = 0;
        rot += 50.f * ts;
        static glm::mat4 transform = glm::mat4(1.f);
        transform = glm::rotate(glm::mat4(1.f), glm::radians(rot), glm::vec3(0.f, 1.f, 0.f));

        m_CameraController.OnUpdate(ts);
        ZPG::RenderCommand::SetClearColor({0.0, 0.0, 0.0, 1.0});
        ZPG::RenderCommand::Clear();

        ZPG::Renderer::BeginDraw(m_CameraController.GetCamera());
            shaderProgram->Bind();
            vao->Bind();
            ZPG::Renderer::Submit(shaderProgram, vao, transform);
        ZPG::Renderer::EndDraw();
    }
    void OnEvent(ZPG::Event& event) override {
        m_CameraController.OnEvent(event);
    }
private:
    ZPG::CameraController m_CameraController;
};

class GUILayer : public ZPG::Layer {
    ZPG::Ref<ZPG::VertexArray> vao;
    ZPG::Ref<ZPG::VertexBuffer> vbo;
    ZPG::Ref<ZPG::IndexBuffer> ibo;
    ZPG::Ref<ZPG::ShaderProgram> shaderProgram;
public:
    GUILayer() : m_CameraController() {
        ZPG::f32 aspectRatio = ZPG::Application::Get().GetWindow().GetAspectRatio();
        ZPG::Camera& camera = m_CameraController.GetCamera();
        camera.SetPerspectiveProjection(camera.GetFOV(), aspectRatio, 0.001f, camera.GetZFar());
        camera.SetPosition(glm::vec3(0.0, 0.0, 1.0));
        camera.SetOrientation(glm::quatLookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)));
    }
    void OnAttach() override {
        ZPG::f32 vertices[] = {
            -0.1f, -0.5f, 0.1f,    1.0, 1.0, 1.0, 1.0,
            0.1f, -0.5f, 0.1f,    1.0, 1.0, 1.0, 1.0,
            0.1f, -0.5f, -0.1f,    1.0, 1.0, 1.0, 1.0,
            -0.1f, -0.5f, -0.1f,    1.0, 1.0, 1.0, 1.0,
        };

        ZPG::u32 indices[] = {
            0, 1, 2,
            0, 2, 3,
        };

        auto vbo = ZPG::VertexBuffer::Create(vertices, sizeof(vertices)/sizeof(*vertices));
        vbo->SetLayout({
            {ZPG::ShaderDataType::Float3, "a_Pos"},
            {ZPG::ShaderDataType::Float4, "a_Color"},
        });

        ibo = ZPG::IndexBuffer::Create(indices, sizeof(indices)/sizeof(*indices));
        vao = ZPG::VertexArray::Create();

        vao->SetIndexBuffer(ibo);
        vao->AddVertexBuffer(vbo);

        shaderProgram = ZPG::ShaderProgram::Create("./assets/shaders/basic.glsl");
    }
    void OnUpdate(ZPG::Timestep ts) override {
        m_CameraController.OnUpdate(ts);

        ZPG_INFO("GUILayer");
        ZPG::Renderer::BeginDraw(m_CameraController.GetCamera());
            shaderProgram->Bind();
            vao->Bind();
            ZPG::Renderer::Submit(shaderProgram, vao);
        ZPG::Renderer::EndDraw();
    }
    void OnEvent(ZPG::Event& event) override {
        m_CameraController.OnEvent(event);
    }
private:
    ZPG::CameraController m_CameraController;
};

void FirstScene::OnAttach() {
    PushLayer(new MainLayer());
    PushLayer(new GUILayer());
}
void FirstScene::OnUpdate(ZPG::Timestep ts) {
    UpdateLayers(ts);
}
void FirstScene::OnEvent(ZPG::Event &event) {
    PropagateEventDownLayers(event);
}