//
// Created by phatt on 10/24/25.
//

#include "PointLightEntity.h"

#include "Light/PointLight.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

    PointLightEntity::PointLightEntity(
        const ref<PointLight>& pointLight,
        const ref<Model>& model,
        const ref<Transform>& transform
    )
        : Entity(model, transform)
        , m_PointLight(pointLight) 
    {
        ZPG_PROFILE_FUNCTION();
        m4 tr = GetTransformMatrix();
        m_PointLight->Position.Set(v3(tr[3]) / tr[3].w);
    }

    PointLightEntity::~PointLightEntity() 
    {
        ZPG_PROFILE_FUNCTION();
    }

    void PointLightEntity::Update(Timestep& ts) 
    {
        ZPG_PROFILE_FUNCTION();
        Entity::Update(ts);
        m4 tr = GetTransformMatrix();
        m_PointLight->Position.Set(v3(tr[3]) / tr[3].w);
    }
}
