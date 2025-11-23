//
// Created by phatt on 10/26/25.
//

#include "SpotLightEntity.h"

#include "Light/SpotLight.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

SpotLightEntity::SpotLightEntity(
    const ref<SpotLight>& spotLight,
    const ref<Model>& model,
    const ref<Transform>& transform
)
: Entity(model, transform)
, m_SpotLight(spotLight)
{
    ZPG_PROFILE_FUNCTION();
}

SpotLightEntity::~SpotLightEntity() {
    ZPG_PROFILE_FUNCTION();
}

void SpotLightEntity::Update(Timestep& ts) {
    ZPG_PROFILE_FUNCTION();
    Entity::Update(ts);
    m4 transform = GetTransformMatrix();
    m_SpotLight->Position.Set(v3(transform[3]) / transform[3].w);
}

}
