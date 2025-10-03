#include "ScaleTransform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ZPG {

ScaleTransform::ScaleTransform(f32 scalar) {
    m_ScaleVector = glm::vec3(scalar);
}
ScaleTransform::ScaleTransform(f32 xScale, f32 yScale, f32 zScale) {
    m_ScaleVector = glm::vec3(xScale, yScale, zScale);
}
ScaleTransform::ScaleTransform(const glm::vec3& scale) {
    m_ScaleVector = scale;
}
glm::mat4 ScaleTransform::GetMatrix() {
    return glm::scale(glm::mat4(1.f), m_ScaleVector);
}

}
