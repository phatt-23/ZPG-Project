#include "ModelLayer.h"
#include "Models/box.h"

using namespace ZPG;
using namespace glm;

namespace TestSkyboxNS {

ModelLayer::ModelLayer() {
}

void ModelLayer::OnAttach() {
    m_BoxVAO = VertexArray::Create({
        VertexBuffer::Create(BoxModel::boxVertices, sizeof(BoxModel::boxVertices), {
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoord"},
        }),
    }, IndexBuffer::Create(BoxModel::boxIndices, ZPG_ARRAY_LENGTH(BoxModel::boxIndices)));

    f32 speed = 90;

    auto sunWorldTr = TransformGroup::Build()
        .Add<Translate>(v3(1.0, 0.0, 0.0))
        .Add<DynRotate>(0.0f, speed, v3(0.0, 1.0, 0.0))
        .Compose();

    auto sunTr = TransformGroup::Build()
        .Add<Scale>(v3(0.4f))
        .Include(sunWorldTr)
        .Compose();

    auto earthWorldTr = TransformGroup::Build()
        .WithParent(sunWorldTr)
        .Add<Translate>(v3(2.0, 0.0, 0.0))
        .Add<DynRotate>(0.0f, 2*speed, v3(0.0, 1.0, 0.0))
        .Compose();

    auto earthTr = TransformGroup::Build()
        .Add<Scale>(v3(0.2f))
        .Include(earthWorldTr)
        .Compose();

    auto moonTr = TransformGroup::Build()
        .WithParent(earthWorldTr)
        .Add<Scale>(0.1f)
        .Add<Translate>(1.0, 0.0, 0.0)
        .Add<DynRotate>(0.0, speed, v3(0.0, 1.0, 0.0))
        .Compose();
    
    m_Scene->GetEntityManager().AddEntity(
        new Entity(m_Scene->GetResourceManager().GetModel("sphere"), sunTr));

    m_Scene->GetEntityManager().AddEntity(
        new Entity(m_Scene->GetResourceManager().GetModel("sphere"), earthTr));

    m_Scene->GetEntityManager().AddEntity(
        new Entity(m_Scene->GetResourceManager().GetModel("sphere"), moonTr));
}

void ModelLayer::OnUpdate(SceneContext& context) {
    for (auto& entity : m_Scene->GetEntityManager().GetEntities()) {
        entity->Update(context.m_Timestep);
    }
}

void ModelLayer::OnRender(const RenderContext& context) {
    for (auto& entity : m_Scene->GetEntityManager().GetEntities()) {
        Renderer::SumbitEntity(*entity);
    }

    for (auto& light : context.m_Lights) {
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
