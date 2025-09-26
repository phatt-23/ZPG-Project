//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"

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
        ZPG_INFO("On attach");

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

        shaderProgram->Bind();
        vao->Bind();
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
            ZPG::Renderer::Submit(shaderProgram, vao, transform);
        ZPG::Renderer::EndDraw();
    }
    void OnEvent(ZPG::Event& event) override {
        m_CameraController.OnEvent(event);
    }
private:
    ZPG::CameraController m_CameraController;
};


class FirstScene : public ZPG::Scene {
public:
    void OnAttach() override {
        PushLayer(new MainLayer());
    }
    
    void OnUpdate(ZPG::Timestep ts) override {
        UpdateLayers(ts);
    }

    void OnEvent(ZPG::Event &event) override {
        PropagateEventDownLayers(event);
    }
private:
};


class ClientApp : public ZPG::Application {
public:
    ClientApp() {
        m_SceneManager.AddScene("First", ZPG::CreateRef<FirstScene>());
    }
private:
    
};


#include "Core/EntryPoint.h"    // main function gets put here
ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}
