#pragma once
#include "Core/Core.h"
#include "Transform/StaticTransform.h"

namespace ZPG {

class Translate : public StaticTransform {
public:
    Translate(f32 xTranslate, f32 yTranslate, f32 zTranslate);
    Translate(const glm::vec3& translation);
    void ComputeMatrix() override;
private:
    glm::vec3 m_TranslationVector;
};

}
