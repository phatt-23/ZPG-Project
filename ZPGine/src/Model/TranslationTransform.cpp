#include "TranslationTransform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ZPG {

TranslationTransform::TranslationTransform(f32 xTranslate, f32 yTranslate, f32 zTranslate) {
    m_TranslationVector = glm::vec3(xTranslate, yTranslate, zTranslate);
}
TranslationTransform::TranslationTransform(const glm::vec3& translation) {
    m_TranslationVector = translation;
}
glm::mat4 TranslationTransform::GetMatrix() {
    return glm::translate(glm::mat4(1.f), m_TranslationVector);
}

}
