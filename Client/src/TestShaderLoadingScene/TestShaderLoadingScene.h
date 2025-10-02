#pragma once
#include "ZPGine.h"
#include "Models/tree.h"
#include <glm/gtc/type_ptr.hpp>

using namespace ZPG;

class TestShaderLoadingScene : public Scene {
public:
    TestShaderLoadingScene() : m_CameraController(), m_Timestep(0.f) {
        f32 aspectRatio = Application::Get().GetWindow().GetAspectRatio();
        Camera& camera = m_CameraController.GetCamera();
        camera.SetPerspectiveProjection(camera.GetFOV(), aspectRatio, 0.001f, camera.GetZFar());
        camera.SetPosition(glm::vec3(0.0, 0.0, 1.0));
        camera.SetOrientation(glm::quatLookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)));
    }
    void OnAttach() override {
        auto vertShader = Shader::CreateFromCode("test.vert", Shader::Vertex, R"(
            #version 330
            layout(location = 0) in vec3 a_Pos;
            layout(location = 1) in vec3 a_Normal;
            uniform mat4 u_Model;
            uniform mat4 u_ViewProj;
            out vec3 v_Normal;
            void main() {
                gl_Position = u_ViewProj * u_Model * vec4(a_Pos, 1.f);
            }
            )");
        
        auto fragShader = Shader::CreateFromCode("red.frag", Shader::Fragment, R"(
            #version 330
            out vec4 f_Color;
            void main() {
                f_Color = vec4(1.0f, 0.f, 0.f, 1.f);
            }
            )");
        auto fragShader2 = Shader::Create("./assets/shaders/normal_as_color.frag");
        auto fragShader3 = Shader::Create("NormalAsColor.frag", "./assets/shaders/normal_as_color.frag");

        std::vector<Ref<Shader>> shaders;
        shaders.push_back(vertShader);
        shaders.push_back(fragShader);

        m_ShaderProgram = ShaderProgram::Create("test.vert + normal_as_color.frag", shaders);
        auto treeVBO = VertexBuffer::Create(tree, sizeof(tree)/sizeof(*tree));
        treeVBO->SetLayout({
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float3, "a_Normal"},
        });
        m_TreeVAO = VertexArray::Create();
        m_TreeVAO->AddVertexBuffer(treeVBO);
    }
    void OnUpdate(Timestep ts) override {
        m_Timestep = ts;
        if (not ImGui::GetIO().WantCaptureKeyboard) 
            m_CameraController.OnUpdate(ts);

        RenderCommand::SetClearColor({0.0, 0.0, 0.0, 1.0});
        RenderCommand::Clear();
        Renderer::BeginDraw(m_CameraController.GetCamera());
            Renderer::Submit(m_ShaderProgram, m_TreeVAO, glm::mat4(1.f));
        Renderer::EndDraw();
    }
    void OnEvent(Event& event) override {
        m_CameraController.OnEvent(event);
    }
private:
    Ref<ShaderProgram> m_ShaderProgram;
    CameraController m_CameraController;
    Ref<VertexArray> m_TreeVAO;
    Timestep m_Timestep;
};