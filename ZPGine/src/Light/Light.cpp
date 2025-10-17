//
// Created by phatt on 10/17/25.
//
#include "Light.h"

namespace ZPG {

Light::Light(LightType lightType)
: m_LightType(lightType) {
}

LightType Light::GetLightType() const {
  return m_LightType;
}

}
