#include "Transform.h"

namespace ZPG
{
    Transform::Transform() 
        : m_Matrix(glm::mat4(1.0f)) 
    {
    }

    Transform::Transform(const glm::mat4& matrix) 
        : m_Matrix(matrix) 
    {
    }

    Transform::~Transform()
    {
    }

    const m4& Transform::GetMatrix() 
    { 
        return m_Matrix; 
    }

    void Transform::Update([[maybe_unused]] Timestep& ts) 
    {
    }

    void Transform::ComputeMatrix()
    {
    }

    v3 Transform::GetScaleVector() const
    {
        return v3(m_Matrix[0][0], m_Matrix[1][1], m_Matrix[2][2]) / m_Matrix[3][3];
    }

    v3 Transform::GetPositionVector() const
    {
        return v3(m_Matrix[3]) / m_Matrix[3].w;
    }

    v3 Transform::GetUpVector() const
    {
        return v3(m_Matrix[1]);
    }
    v3 Transform::GetRightVector() const
    {
        return v3(m_Matrix[0]);
    }
    v3 Transform::GetFrontVector() const
    {
        return v3(m_Matrix[2]);
    }
}
