#pragma once
#include "Transform/DynTransform.h"

namespace ZPG {

class DynTranslationTransform : public DynamicTransform {
    using crVEC3 = const glm::vec3&;
    using VEC3 = glm::vec3;
public:
    DynTranslationTransform(
        crVEC3 startingTranslation, 
        crVEC3 translationIncrement, 
        crVEC3 minTranslation, 
        crVEC3 maxTranslation);

    void Update(Timestep ts) override;
private:
    void ComputeMatrix() override;
private:
    VEC3 m_CurrentTranslation;
    VEC3 m_TranslationIncrement;
    VEC3 m_MinTranslation;
    VEC3 m_MaxTranslation;
};

}
