#include "SphereLayer.h"
#include "Model/RotationTransform.h"
#include "Model/ScaleTransform.h"
#include "Model/TranslationTransform.h"
#include "Models/sphere.h"

using namespace ZPG;

SphereLayer::SphereLayer(ZPG::CameraController& cameraController) : m_CameraController(cameraController) {

}
void SphereLayer::OnAttach() {
    m_SphereVAO = VertexArray::Create();
    m_NormalShaderProgram = ShaderProgram::Create("./assets/shaders/normal.glsl");
    
    Ref<VertexBuffer> vbo = VertexBuffer::Create(sphere, ZPG_ARRAYSIZE(sphere));
    vbo->SetLayout({
        {ShaderDataType::Float3, "a_Pos"},
        {ShaderDataType::Float3, "a_Normal"},
    });

    m_SphereVAO->AddVertexBuffer(vbo);
}
void SphereLayer::OnUpdate(ZPG::Timestep ts) {
    using namespace glm;
    m_Timestep = ts;

    static f32 rot = 0;
    rot += 50.f * ts;

    // to rotate A around B
    // 1. get vector c = pos(A) - pos(B)
    // 2. A goes back to origin (0,0,0) by vector d = -pos(A)
    // 3. A translate by c
    // 4. A rotate around origin
    // 5. A translate by d

    vec3 worldUp(1.0, 0.0, 0.0);

    Ref<CompoundTransform> foo = CreateRef<CompoundTransform>();
    foo->Emplace<ScaleTransform>(0.6f);
    foo->Emplace<RotationTransform>(rot, vec3(1.0, 1.0, 0.0));
    foo->Emplace<TranslationTransform>(vec3(0.0, 0.0, 0.0));

    CompoundTransform sun;
    sun.Emplace<ScaleTransform>(0.6f);
    sun.Emplace<RotationTransform>(rot, vec3(1.0, 1.0, 0.0));
    sun.Emplace<TranslationTransform>(vec3(0.0, 0.0, 0.0));
    sun.Push(foo);
    glm::mat4 modelMatrix = sun.GetMatrix();

    // earth
    vec3 earthRotationAxis(1.0, 0.0, 1.0);
    vec3 earthRight = cross(normalize(earthRotationAxis), normalize(worldUp));
    vec3 earthFront = cross(normalize(earthRight), normalize(earthRotationAxis));

    CompoundTransform earth;
    // local
    earth.Emplace<ScaleTransform>(0.5f);
    earth.Emplace<TranslationTransform>(2.f * earthFront);
    earth.Emplace<RotationTransform>(4 * rot, earthRotationAxis);

    // // moon
    vec3 moonAxis(1.0, 1.0, 0.0);
    vec3 moonRight = cross(normalize(moonAxis), normalize(worldUp));
    vec3 front = cross(normalize(moonAxis), normalize(moonRight));
    CompoundTransform moon;
    moon.Emplace<ScaleTransform>(0.25f);
    moon.Emplace<TranslationTransform>(2.f * front);
    moon.Emplace<RotationTransform>(5 * rot, moonAxis);
    moon.Emplace<TranslationTransform>(vec3(earth.GetMatrix()[3]));


    Renderer::BeginDraw(m_CameraController.GetCamera());
        Renderer::Submit(m_NormalShaderProgram, m_SphereVAO, sun.GetMatrix());
        Renderer::Submit(m_NormalShaderProgram, m_SphereVAO, earth.GetMatrix());
        Renderer::Submit(m_NormalShaderProgram, m_SphereVAO, moon.GetMatrix());
    Renderer::EndDraw();
}

