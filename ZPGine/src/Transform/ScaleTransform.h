#pragma once
#include "Transformation.h"
#include "Core/Core.h"

namespace ZPG {

class ScaleTransform : public Transform {
public:
    ScaleTransform(f32 scalar);
    ScaleTransform(f32 xScale, f32 yScale, f32 zScale);
    ScaleTransform(const glm::vec3& scale);
    glm::mat4 GetMatrix() override;
private:
    glm::vec3 m_ScaleVector;
};

}
