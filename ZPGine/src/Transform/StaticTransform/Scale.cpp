#include "Scale.h"
#include "Transform/StaticTransform.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Profiling/Instrumentor.h"

namespace ZPG {

Scale::Scale(f32 scalar)
    : StaticTransform(false)
    , m_ScaleVector(scalar)
{
    ZPG_PROFILE_FUNCTION();
    ComputeMatrix();
}

Scale::Scale(f32 xScale, f32 yScale, f32 zScale)
    : StaticTransform(false)
    , m_ScaleVector(xScale, yScale, zScale)
{
    ZPG_PROFILE_FUNCTION();
    ComputeMatrix();
}

Scale::Scale(const v3& scale) : StaticTransform(false){
    ZPG_PROFILE_FUNCTION();
    m_ScaleVector = scale;
    ComputeMatrix();
}

void Scale::ComputeMatrix() {
    ZPG_PROFILE_FUNCTION();
    m_Matrix = glm::scale(m4(1.0f), m_ScaleVector);
    m_Cached = true;
}

const v3& Scale::GetScale() const {
    ZPG_PROFILE_FUNCTION();
    return m_ScaleVector;
}

void Scale::SetScale(const v3& scale) {
    ZPG_PROFILE_FUNCTION();
    m_ScaleVector = scale;
    ComputeMatrix();
}

}
