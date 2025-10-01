#pragma once
#include "Transformation.h"
#include "Core/Core.h"

namespace ZPG {

class TranslationTransform : public Transform {
public:
    TranslationTransform(f32 xTranslate, f32 yTranslate, f32 zTranslate);
    TranslationTransform(const glm::vec3& translation);
    glm::mat4 GetMatrix() override;
private:
    glm::vec3 m_TranslationVector;
};

}
