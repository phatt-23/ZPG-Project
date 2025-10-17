#include "ModelLayer.h"
#include "../assets/models/phatt/box.h"

using namespace ZPG;

namespace TransformHierarchyNS {

ModelLayer::ModelLayer() {
}

void ModelLayer::OnAttach() {
    m_BoxVAO = VertexArray::Create({
        VertexBuffer::Create(phatt::boxVertices, std::size(phatt::boxVertices), {
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"},
        }),
    }, IndexBuffer::Create(phatt::boxIndices, std::size(phatt::boxIndices)));

    f32 speed = 90;

    auto sunWorld = TransformGroup::Build()
        .Add<Translate>(v3(1.0, 0.0, 0.0))
        .Add<DynRotate>(0.0f, speed, v3(0.0, 1.0, 0.0))
        .Compose();

    auto sun = TransformGroup::Build()
        .Add<Scale>(v3(0.4f))
        .Include(sunWorld)
        .Compose();

    auto earthWorld = TransformGroup::Build()
        .WithParent(sunWorld)
        .Add<Translate>(v3(2.0, 0.0, 0.0))
        .Add<DynRotate>(0.0f, 2*speed, v3(0.0, 1.0, 0.0))
        .Compose();

    auto earth = TransformGroup::Build()
        .Add<Scale>(v3(0.2f))
        .Include(earthWorld)
        .Compose();

    auto moon = TransformGroup::Build()
        .WithParent(earthWorld)
        .Add<Scale>(0.1f)
        .Add<Translate>(1.0, 0.0, 0.0)
        .Add<DynRotate>(0.0, speed, v3(0.0, 1.0, 0.0))
        .Compose();

    m_Scene->GetEntityManager().AddEntity(new Entity(
        m_Scene->GetResourceManager().GetModel("sphere"),
        sun));

    m_Scene->GetEntityManager().AddEntity(new Entity(
        m_Scene->GetResourceManager().GetModel("sphere"),
        earth));

    m_Scene->GetEntityManager().AddEntity(new Entity(
        m_Scene->GetResourceManager().GetModel("sphere"),
        moon));
}

void ModelLayer::OnUpdate(SceneContext& context) {
    for (auto& entity : m_Scene->GetEntityManager().GetEntities()) {
        entity->Update(context.Ts);
    }
}

void ModelLayer::OnRender(const RenderContext& context) {
    for (auto& entity : m_Scene->GetEntityManager().GetEntities()) {
        Renderer::SubmitEntity(*entity);
    }

    for (auto& light : context.Lights) {
        if (light->GetLightType() == LightType::Point) {
            auto pointLight = (PointLight*)light.get();

            auto transform = TransformGroup::Create();
            transform->Add<Scale>(0.4f * v3(1.0));
            transform->Add<Translate>(pointLight->GetPosition());

            Renderer::Submit(
                *m_Scene->GetResourceManager().GetShaderProgram("basic_single_color"),
                *m_BoxVAO,
                transform->GetMatrix()
            );
        } else if (light->GetLightType() == LightType::Spotlight) {
            auto spotlight = (SpotLight*)light.get();

            auto transform = TransformGroup::Create();
            transform->Add<Scale>(v3(1.0, 1.0, 1.0));
            transform->Add<Translate>(spotlight->GetPosition());

            Renderer::Submit(
                *m_Scene->GetResourceManager().GetShaderProgram("basic_single_color"),
                *m_BoxVAO,
                transform->GetMatrix()
            );
        }
    }
}


}
