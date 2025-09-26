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
        camera.SetPerspectiveProjection(camera.GetFOV(), aspectRatio, camera.GetZNear(), camera.GetZFar());
        camera.SetPosition(glm::vec3(0.0, 0.0, 1.0));
        camera.SetOrientation(glm::quatLookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)));
    }
    void OnAttach() override { 
        ZPG_INFO("On attach");
        ZPG::u32 indices[] = {
            // top
            0, 1, 2,
            0, 2, 3,

            // back
            4, 5, 6,
            4, 6, 7,
        };

        ZPG::f32 vertices[] = {
            // position         // color
            -0.5f, -0.5f, 0.5f,  1.f, 0.f, 0.f, 1.f,  // front bottom left
            0.5f, -0.5f, 0.5f,  0.f, 1.f, 0.f, 1.f,  // front bottom right 
            0.5f, 0.5f, 0.5f,  0.f, 0.f, 1.f, 1.f, // front top right
            -0.5f, 0.5f, 0.5f,  1.f, 1.f, 0.f, 1.f, // front top left
            
            -0.5f, -0.5f, -0.5f,  1.f, 0.f, 0.f, 1.f, // back left bottom
            0.5f, -0.5f, -0.5f,  0.f, 1.f, 0.f, 1.f, // back bottom right
            0.5f, 0.5f, -0.5f,  0.f, 0.f, 1.f, 1.f, // back top right
            -0.5f, 0.5f, -0.5f,  1.f, 1.f, 0.f, 1.f, // back top left
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
        m_CameraController.OnUpdate(ts);
        ZPG::RenderCommand::SetClearColor({1.0, 0.0, 0.0, 1.0});
        ZPG::RenderCommand::Clear();

        ZPG::Renderer::BeginDraw(m_CameraController.GetCamera());
            ZPG::Renderer::Submit(shaderProgram, vao);
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
