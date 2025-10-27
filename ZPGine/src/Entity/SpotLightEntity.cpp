//
// Created by phatt on 10/26/25.
//

#include "SpotLightEntity.h"

#include "Light/SpotLight.h"

namespace ZPG {

SpotLightEntity::SpotLightEntity(
    const ref<SpotLight>& spotLight,
    const ref<Model>& model,
    const ref<Transform>& transform
)
: Entity(model, transform)
, m_SpotLight(spotLight) {

}

SpotLightEntity::~SpotLightEntity() {
}

void SpotLightEntity::Update(Timestep& ts) {
    Entity::Update(ts);
    m_SpotLight->m_Position.SetPosition(GetTransformMatrix()[3]);
    // m_SpotLight->m_Direction.SetDirection(GetTransformMatrix()[2]);
}

}
