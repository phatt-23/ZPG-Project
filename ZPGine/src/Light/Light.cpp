//
// Created by phatt on 10/17/25.
//
#include "Light.h"

#include "Profiling/Instrumentor.h"

namespace ZPG {

Light::Light(LightType lightType)
: m_LightType(lightType) {
}

LightType Light::GetLightType() const {
    ZPG_PROFILE_FUNCTION();
    return m_LightType;
}

}
