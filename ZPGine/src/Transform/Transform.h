#pragma once
#include "Core/Timestep.h"
#include <glm/glm.hpp>

namespace ZPG {

class Transform {
public:
    Transform() : m_Matrix(glm::mat4(1.0f)) {}
    Transform(const glm::mat4& matrix) : m_Matrix(matrix) {}
    virtual ~Transform() = default;

    virtual const glm::mat4& GetMatrix() { return m_Matrix; }
    virtual void Update([[maybe_unused]] Timestep ts) {}
protected:
    virtual void ComputeMatrix() {}

    glm::mat4 m_Matrix;
};

}

