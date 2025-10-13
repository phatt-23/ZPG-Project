#include "ModelLayer.h"
#include "Models/box.h"

using namespace ZPG;
using namespace glm;

namespace TestModelLoadingNS {

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

    auto transform = TransformGroup::Create();
    transform->Add<Scale>(0.01f * vec3(1.0f));
    transform->Add<Translate>(vec3(0.0f, 0.8f, 0.0f));
    transform->Add<Rotate>(180.0f, vec3(1.0f, 0.0f, 0.0f));
    transform->Add<DynRotate>(0.0f, 20.0f, vec3(0.0, 1.0, 0.0));
    transform->Add<DynRotate>(0.0f, 20.0f, vec3(1.0, 0.0, 0.0));

    m_Scene->GetEntityManager().AddEntity(new Entity(
    m_Scene->GetResourceManager().GetModel("hyena"), transform));

    for (int i = 0; i < 10; i++) {
        auto tr = TransformGroup::Create();
        tr->Add<Scale>(0.01f * vec3(1.0f));
        tr->Add<DynRotate>(0.0f, 50.0f, vec3(0.0, 1.0, 0.0));
        tr->Add<Translate>(1.0f, 1.0f, (float)i * 3.0f);

        m_Scene->GetEntityManager().AddEntity(new Entity(
            m_Scene->GetResourceManager().GetModel("hyena"), tr));

        auto tr2 = TransformGroup::Create();
        tr2->Add<Scale>(0.02f * vec3(1.0f));
        tr2->Add<Rotate>(180.f, vec3(1, 0, 0));
        tr2->Add<DynRotate>(0.0f, 50.0f, vec3(0.0, 1.0, 0.0));
        tr2->Add<Translate>((float)i * 3.0f, -2.0f, 0.0f);

        m_Scene->GetEntityManager().AddEntity(new Entity(
            m_Scene->GetResourceManager().GetModel("hyena"), tr2));
    }

    auto mustangTr = TransformGroup::Create();
    mustangTr->Add<Scale>(100.0f);
    mustangTr->Add<Rotate>(90.0f, vec3(1.0, 0.0, 0.0));
    mustangTr->Add<Translate>(vec3(-3.0, -0.2, 0.0));

    m_Scene->GetEntityManager().AddEntity(new Entity(
        m_Scene->GetResourceManager().GetModel("ford_mustang"), mustangTr));
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
            transform->Add<Scale>(0.4f * vec3(1.0));
            transform->Add<Translate>(pointLight->GetPosition());

            Renderer::Submit(
                *m_Scene->GetResourceManager().GetShaderProgram("basic_single_color"),
                *m_BoxVAO,
                transform->GetMatrix()
            );
        } else if (light->GetLightType() == LightType::Spotlight) {
            auto spotlight = (SpotLight*)light.get();

            auto transform = TransformGroup::Create();
            transform->Add<Scale>(vec3(1.0, 1.0, 1.0));
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
