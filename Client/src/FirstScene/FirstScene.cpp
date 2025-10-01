#include "FirstScene.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

using namespace ZPG;

f32 random_f32(f32 min, f32 max) {
    return (max - min) * (f32)rand() / (f32)RAND_MAX + min;
}

f32 boxVertices[] = {
    -0.1f, -0.1f,  0.1f,    1.0, 0.0, 0.0, 1.0,
    -0.1f, -0.1f, -0.1f,    0.0, 1.0, 0.0, 1.0,
    0.1f, -0.1f, -0.1f,    0.0, 0.0, 1.0, 1.0,
    0.1f, -0.1f,  0.1f,    1.0, 0.0, 0.0, 1.0,
    -0.1f,  0.1f,  0.1f,    0.0, 1.0, 0.0, 1.0,
    -0.1f,  0.1f, -0.1f,    0.0, 0.0, 1.0, 1.0,
    0.1f,  0.1f, -0.1f,    1.0, 1.0, 1.0, 1.0,
    0.1f,  0.1f,  0.1f,    0.0, 1.0, 1.0, 1.0,
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
    -0.1f, -0.5f, 0.1f,    1.0, 1.0, 1.0, 1.0,
    0.1f, -0.5f, 0.1f,    1.0, 1.0, 1.0, 1.0,
    0.1f, -0.5f, -0.1f,    1.0, 1.0, 1.0, 1.0,
    -0.1f, -0.5f, -0.1f,    1.0, 1.0, 1.0, 1.0,
};

u32 planeIndices[] = {
    0, 1, 2,
    0, 2, 3,
};


class MyLayer : public Layer {
    Ref<VertexArray> m_BoxVao;
    Ref<VertexArray> m_PlaneVao;
    Ref<ShaderProgram> m_BasicShaderProgram;
    Ref<ShaderProgram> m_RedShaderProgram;
    Timestep m_Timestep = 0;
    std::vector<glm::mat4> m_Transforms;
public:
    MyLayer() : m_CameraController() {
        f32 aspectRatio = Application::Get().GetWindow().GetAspectRatio();
        Camera& camera = m_CameraController.GetCamera();
        camera.SetPerspectiveProjection(camera.GetFOV(), aspectRatio, 0.001f, camera.GetZFar());
        camera.SetPosition(glm::vec3(0.0, 0.0, 1.0));
        camera.SetOrientation(glm::quatLookAt(glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)));
    }
    void OnAttach() override {
        m_BasicShaderProgram = ShaderProgram::Create("./assets/shaders/basic.glsl");
        m_RedShaderProgram = ShaderProgram::Create("./assets/shaders/red.glsl");

        auto boxVbo = VertexBuffer::Create(boxVertices, ZPG_ARRAYSIZE(boxVertices));
        boxVbo->SetLayout({
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float4, "a_Color"},
        });

        auto boxIbo = IndexBuffer::Create(boxIndices, ZPG_ARRAYSIZE(boxIndices));

        m_BoxVao = VertexArray::Create();
        m_BoxVao->AddVertexBuffer(boxVbo);
        m_BoxVao->SetIndexBuffer(boxIbo);
        
        auto planeVbo = VertexBuffer::Create(planeVertices, ZPG_ARRAYSIZE(planeVertices));
        planeVbo->SetLayout({
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float4, "a_Color"},
        });

        auto planeIbo = IndexBuffer::Create(planeIndices, ZPG_ARRAYSIZE(planeIndices));
        m_PlaneVao = VertexArray::Create();
        m_PlaneVao->AddVertexBuffer(planeVbo);
        m_PlaneVao->SetIndexBuffer(planeIbo);

        for (int i = 0; i < 1000; i++)
            m_Transforms.push_back(glm::translate(glm::mat4(1.f), glm::vec3(random_f32(-10.f, 10.f), random_f32(-10.f, 10.f), random_f32(-10.f, 10.f))));
    }
    void OnUpdate(Timestep ts) override {
        m_Timestep = ts;

        // If ImGui wants keyboard, don't let controller "move"
        if (not ImGui::GetIO().WantCaptureKeyboard) 
            m_CameraController.OnUpdate(ts);

        static f32 rot = 0;
        rot += 50.f * ts;

        using namespace glm;

        mat4 rotMat = rotate(mat4(1.f), radians(rot), vec3(0.f, 1.f, 0.f));
        mat4 trMat  = translate(mat4(1.f), vec3(1.0, 0.0, 0.0));
        mat4 scMat = scale(mat4(1.f), vec3(2.0, 2.0, 2.0));
        mat4 boxTransform = rotMat * trMat * rotMat * scMat;

        static glm::mat4 planeTransform = glm::scale(glm::mat4(1.f), glm::vec3(10.f, 1.0f, 20.f));

        RenderCommand::SetClearColor({0.0, 0.0, 0.0, 1.0});
        RenderCommand::Clear();

        Renderer::BeginDraw(m_CameraController.GetCamera());
            Renderer::Submit(m_BasicShaderProgram, 
                             m_BoxVao, 
                             boxTransform);
            Renderer::Submit(m_BasicShaderProgram, 
                             m_BoxVao, 
                             scMat * boxTransform);
            Renderer::Submit(m_BasicShaderProgram, 
                             m_BoxVao, 
                             boxTransform * scMat);
            Renderer::Submit(m_BasicShaderProgram, 
                             m_BoxVao, 
                             boxTransform * scMat);
            Renderer::Submit(m_BasicShaderProgram, 
                             m_BoxVao, 
                             scMat * translate(mat4(1.f), vec3(10.0, 0, 0)) * scMat);
            Renderer::Submit(m_RedShaderProgram, m_PlaneVao, planeTransform);

            for (auto& tr : m_Transforms) {
                Renderer::Submit(m_BasicShaderProgram, m_BoxVao, tr);
            }
        Renderer::EndDraw();
    }
    void OnEvent(Event& event) override {
        m_CameraController.OnEvent(event);
    }
    void OnImGuiRender() override {
        ImGui::Begin("FirstScene Data");
            ImGui::Text("FPS: %.4f", 1.f/m_Timestep);
            glm::vec3 cPos = m_CameraController.GetCamera().GetPosition();
            glm::quat cOr = m_CameraController.GetCamera().GetOrientation();
            ImGui::InputFloat3("Camera position", glm::value_ptr(cPos));
            ImGui::InputFloat4("Camera orientation", glm::value_ptr(cOr));
        ImGui::End();
    }
private:
    CameraController m_CameraController;
};


void FirstScene::OnAttach() {
    PushLayer(new MyLayer());
}
void FirstScene::OnUpdate(Timestep ts) {
    UpdateLayers(ts);
}
void FirstScene::OnEvent(Event &event) {
    PropagateEventDownLayers(event);
}

