//
// Created by phatt on 10/16/25.
//

#include "SpheresLayer.h"
#include "../assets/models/nemec/sphere.h"

using namespace ZPG;

namespace PBRSpheresSceneNS {

void SpheresLayer::OnAttach() {

}

void SpheresLayer::OnUpdate(ZPG::SceneContext& ctx) {
    for (auto& ent : m_Scene->GetEntityManager().GetEntities()) {
        ent->Update(ctx.Ts);
    }
}

void SpheresLayer::OnRender(const ZPG::RenderContext& ctx) {
    for (auto& ent : m_Scene->GetEntityManager().GetEntities()) {
        Renderer::SubmitEntity(*ent);
    }

    for (auto& light : ctx.Lights) {
        if (light->GetLightType() == LightType::Point || light->GetLightType() == LightType::Spotlight) {
            auto pointLight = (PointLight*)light.get();

            auto transform = TransformGroup::Create();
            transform->Add<Scale>(0.4f * v3(1.0));
            transform->Add<Translate>(pointLight->m_Position.GetPosition());

            Renderer::Submit(
                *m_Scene->GetResourceManager().GetShaderProgram("basic_single_color"),
                *m_Scene->GetResourceManager().GetModel(CommonResources::MODEL_BOX),
                transform->GetMatrix()
            );
        }
    }

}

} // BallsSceneNS