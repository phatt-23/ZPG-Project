//
// Created by phatt on 10/24/25.
//

#include "PointLightEntity.h"

#include "Light/PointLight.h"

namespace ZPG {

PointLightEntity::PointLightEntity(
    const ref<PointLight>& pointLight,
    const ref<Model>& model,
    const ref<Transform>& transform)
: Entity(model, transform)
, m_PointLight(pointLight) {

}

PointLightEntity::~PointLightEntity() {
}

void PointLightEntity::Update(Timestep& ts) {
    Entity::Update(ts);
    m_PointLight->m_Position.SetPosition(GetTransformMatrix()[3]);
}

}
