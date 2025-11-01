#pragma once

#include "Transform/StaticTransform.h"

namespace ZPG {

class Translate : public StaticTransform {
public:
    Translate(f32 xTranslate, f32 yTranslate, f32 zTranslate);
    Translate(const glm::vec3& translation);
    void ComputeMatrix() override;

    void SetTranslation(const v3& translation);
    const v3& GetTranslation() const;
private:
    v3 m_TranslationVector;
};

}
