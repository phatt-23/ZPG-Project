#include "SphereLayer.h"
#include "Models/sphere.h"

using namespace ZPG;

SphereLayer::SphereLayer(ZPG::CameraController& cameraController) : m_CameraController(cameraController) {

}
void SphereLayer::OnAttach() {
    m_SphereVAO = VertexArray::Create();
    m_NormalShaderProgram = ShaderProgram::Create("./assets/shaders/basic_normal.glsl");
    
    Ref<VertexBuffer> vbo = VertexBuffer::Create(sphere, sizeof(sphere));
    vbo->SetLayout({
        {ShaderDataType::Float3, "a_Pos"},
        {ShaderDataType::Float3, "a_Normal"},
    });

    m_SphereVAO->AddVertexBuffer(vbo);
}
void SphereLayer::OnUpdate(SceneContext& context) {
    using namespace glm;
    m_Timestep = context.m_Timestep;

    static f32 rot = 0;
    rot += 50.f * m_Timestep;

    // to rotate A around B
    // 1. get vector c = pos(A) - pos(B)
    // 2. A goes back to origin (0,0,0) by vector d = -pos(A)
    // 3. A translate by c
    // 4. A rotate around origin
    // 5. A translate by d

    vec3 worldUp(1.0, 0.0, 0.0);

    Ref<TransformGroup> foo = CreateRef<TransformGroup>();
    foo->Add<Scale>(0.6f);
    foo->Add<Rotate>(rot, vec3(1.0, 1.0, 0.0));
    foo->Add<Translate>(vec3(0.0, 0.0, 0.0));

    TransformGroup sun;
    sun.Add<Scale>(0.6f);
    sun.Add<Rotate>(rot, vec3(1.0, 1.0, 0.0));
    sun.Add<Translate>(vec3(0.0, 0.0, 0.0));
    sun.Include(foo);

    // earth
    vec3 earthRotationAxis(1.0, 0.0, 1.0);
    vec3 earthRight = cross(normalize(earthRotationAxis), normalize(worldUp));
    vec3 earthFront = cross(normalize(earthRight), normalize(earthRotationAxis));

    TransformGroup earth;
    // local
    earth.Add<Scale>(0.5f);
    earth.Add<Translate>(2.f * earthFront);
    earth.Add<Rotate>(4 * rot, earthRotationAxis);

    // // moon
    vec3 moonAxis(1.0, 1.0, 0.0);
    vec3 moonRight = cross(normalize(moonAxis), normalize(worldUp));
    vec3 front = cross(normalize(moonAxis), normalize(moonRight));
    TransformGroup moon;
    moon.Add<Scale>(0.25f);
    moon.Add<Translate>(2.f * front);
    moon.Add<Rotate>(5 * rot, moonAxis);
    moon.Add<Translate>(vec3(earth.GetMatrix()[3]));

    Renderer::Submit(*m_NormalShaderProgram, *m_SphereVAO, sun.GetMatrix());
    Renderer::Submit(*m_NormalShaderProgram, *m_SphereVAO, earth.GetMatrix());
    Renderer::Submit(*m_NormalShaderProgram, *m_SphereVAO, moon.GetMatrix());
}

