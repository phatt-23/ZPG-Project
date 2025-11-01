//
// Created by phatt on 10/30/25.
//

#ifndef WORKSPACE_ATTEN_COMPONENT_H
#define WORKSPACE_ATTEN_COMPONENT_H

namespace ZPG {

class AttenComponent {
public:
    AttenComponent(f32 quadratic = 0.9, f32 linear = 0.9, f32 constant = 1.0);

    f32 GetQuadratic() const;
    void SetQuadratic(f32 quadratic);

    f32 GetLinear() const;
    void SetLinear(f32 linear);

    f32 GetConstant() const;
    void SetConstant(f32 constant);

    void SetAttenuation(const v3& attenuation);
    const v3& GetAttenuation() const;

private:
    v3 m_Attenuation;
};

} // ZPG

#endif