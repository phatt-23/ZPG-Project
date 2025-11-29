//
// Created by phatt on 10/30/25.
//

#include "AttenComponent.h"

namespace ZPG 
{
    AttenComponent::AttenComponent(f32 quadratic, 
                                   f32 linear, 
                                   f32 constant)
        : m_Attenuation(quadratic, linear, constant)
        , m_Intensity(1.0)
    {
    }

    AttenComponent::AttenComponent(f32 intensity, 
                                   v3 attenuationCoeficients)
        : m_Attenuation(attenuationCoeficients)
        , m_Intensity(intensity)
    {
    }

    f32 AttenComponent::GetQuadratic() const {
        return m_Attenuation.x;
    }

    void AttenComponent::SetQuadratic(const f32 quadratic) {
        m_Attenuation.x = quadratic;
    }

    f32 AttenComponent::GetLinear() const {
        return m_Attenuation.y;
    }

    void AttenComponent::SetLinear(const f32 linear) {
        m_Attenuation.y = linear;
    }

    f32 AttenComponent::GetConstant() const {
        return m_Attenuation.z;
    }

    void AttenComponent::SetConstant(const f32 constant) {
        m_Attenuation.z = constant;
    }

    void AttenComponent::SetAttenuation(const v3& attenuation) {
        m_Attenuation = attenuation;
    }

    const v3& AttenComponent::GetAttenuation() const {
        return m_Attenuation;
    }
} // ZPG
