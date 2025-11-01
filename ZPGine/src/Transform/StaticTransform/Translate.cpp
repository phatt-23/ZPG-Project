#include "Translate.h"
#include "Transform/StaticTransform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ZPG {

Translate::Translate(f32 xTranslate, f32 yTranslate, f32 zTranslate) 
: StaticTransform(false)
, m_TranslationVector(xTranslate, yTranslate, zTranslate)
{
    ComputeMatrix();
}

Translate::Translate(const glm::vec3& translation) 
: StaticTransform(false)
, m_TranslationVector(translation)
{
    ComputeMatrix();
}

void Translate::ComputeMatrix() {
    m_Matrix = glm::translate(m4(1.0f), m_TranslationVector);
    m_Cached = true;
}


const v3& Translate::GetTranslation() const {
    return m_TranslationVector;
}

void Translate::SetTranslation(const v3& translation) {
    m_TranslationVector = translation;
    ComputeMatrix();
}

}
