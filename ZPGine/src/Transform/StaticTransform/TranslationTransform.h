#pragma once
#include "Core/Core.h"
#include "Transform/StaticTransform.h"

namespace ZPG {

class TranslationTransform : public StaticTransform {
public:
    TranslationTransform(f32 xTranslate, f32 yTranslate, f32 zTranslate);
    TranslationTransform(const glm::vec3& translation);
    void ComputeMatrix() override;
private:
    glm::vec3 m_TranslationVector;
};

}
