//
// Created by phatt on 10/17/25.
//
#include "Light.h"

namespace ZPG {

Light::Light(LightType lightType, const ref<Transform>& transform)
: m_LightType(lightType)
, m_Transform(transform) {
}

LightType Light::GetLightType() const {
    return m_LightType;
}

void Light::SetTransform(ref<Transform> const& transform) {
    m_Transform = transform;
}

const ref<Transform>& Light::GetTransform() const {
    return m_Transform;
}

}
