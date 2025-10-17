#include "Scale.h"
#include "Transform/StaticTransform.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ZPG {

Scale::Scale(f32 scalar)
: StaticTransform(false)
, m_ScaleVector(scalar)
{
    ComputeMatrix();
}

Scale::Scale(f32 xScale, f32 yScale, f32 zScale)
: StaticTransform(false)
, m_ScaleVector(xScale, yScale, zScale)
{
    ComputeMatrix();
}

Scale::Scale(const v3& scale) : StaticTransform(false){
    m_ScaleVector = scale;
    ComputeMatrix();
}

void Scale::ComputeMatrix() {
    m_Matrix = glm::scale(m4(1.0f), m_ScaleVector);
    m_Cached = true;
}

}
