#include "Translate.h"
#include "Transform/StaticTransform.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Profiling/Instrumentor.h"

namespace ZPG {

Translate::Translate(f32 xTranslate, f32 yTranslate, f32 zTranslate) 
: StaticTransform(false)
, m_TranslationVector(xTranslate, yTranslate, zTranslate)
{
    ZPG_PROFILE_FUNCTION();
    ComputeMatrix();
}

Translate::Translate(const glm::vec3& translation) 
: StaticTransform(false)
, m_TranslationVector(translation)
{
    ZPG_PROFILE_FUNCTION();
    ComputeMatrix();
}

void Translate::ComputeMatrix() {
    ZPG_PROFILE_FUNCTION();
    m_Matrix = glm::translate(m4(1.0f), m_TranslationVector);
    m_Cached = true;
}


const v3& Translate::GetTranslation() const {
    ZPG_PROFILE_FUNCTION();
    return m_TranslationVector;
}

void Translate::SetTranslation(const v3& translation) {
    ZPG_PROFILE_FUNCTION();
    m_TranslationVector = translation;
    ComputeMatrix();
}

}
