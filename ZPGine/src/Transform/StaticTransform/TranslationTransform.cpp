#include "TranslationTransform.h"
#include "Transform/StaticTransform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ZPG {

TranslationTransform::TranslationTransform(f32 xTranslate, f32 yTranslate, f32 zTranslate) 
: StaticTransform(false){
    m_TranslationVector = glm::vec3(xTranslate, yTranslate, zTranslate);
    ComputeMatrix();
}
TranslationTransform::TranslationTransform(const glm::vec3& translation) 
: StaticTransform(false){
    m_TranslationVector = translation;
    ComputeMatrix();
}
void TranslationTransform::ComputeMatrix() {
    m_Matrix = glm::translate(glm::mat4(1.0f), m_TranslationVector);
    m_Cached = true;
}

}
