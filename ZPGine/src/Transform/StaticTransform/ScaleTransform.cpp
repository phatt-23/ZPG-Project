#include "ScaleTransform.h"
#include "Transform/StaticTransform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ZPG {

ScaleTransform::ScaleTransform(f32 scalar) : StaticTransform(false) {
    m_ScaleVector = glm::vec3(scalar);
    ComputeMatrix();
}
ScaleTransform::ScaleTransform(f32 xScale, f32 yScale, f32 zScale) : StaticTransform(false){
    m_ScaleVector = glm::vec3(xScale, yScale, zScale);
    ComputeMatrix();
}
ScaleTransform::ScaleTransform(const glm::vec3& scale) : StaticTransform(false){
    m_ScaleVector = scale;
    ComputeMatrix();
}
void ScaleTransform::ComputeMatrix() {
    m_Matrix = glm::scale(glm::mat4(1.0f), m_ScaleVector);
    m_Cached = true;
}

}
