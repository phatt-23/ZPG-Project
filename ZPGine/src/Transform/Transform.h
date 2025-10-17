#pragma once
#include "Core/Timestep.h"
#include <glm/glm.hpp>

namespace ZPG {

class Transform {
public:
    Transform() : m_Matrix(glm::mat4(1.0f)) {}
    Transform(const glm::mat4& matrix) : m_Matrix(matrix) {}
    virtual ~Transform() = default;

    // Gets the computed m_Matrix.
    // User must call either Update or ComputeMatrix to update/compute the m_Matrix.
    // (should be const, but left un-const for flexibility)
    virtual const glm::mat4& GetMatrix() { return m_Matrix; }

    // Called in every iteration of OnUpdate() in a Scene
    virtual void Update([[maybe_unused]] Timestep ts) {}

    // Computes the m_Matrix.
    virtual void ComputeMatrix() {}
protected:
    glm::mat4 m_Matrix;
};

}

