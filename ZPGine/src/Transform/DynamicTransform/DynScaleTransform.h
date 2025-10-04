#pragma once
#include "Core/Timestep.h"
#include "Transform/DynTransform.h"

namespace ZPG {

class DynScaleTransform : public DynamicTransform {
public:
    DynScaleTransform(
        const glm::vec3& startingScale, 
        const glm::vec3& growth, 
        const glm::vec3& minScale, 
        const glm::vec3& maxScale);

    virtual void Update(Timestep ts) override;

protected:
    void ComputeMatrix() override;
    
protected:
    glm::vec3 m_CurrentScale;
    glm::vec3 m_Growth; 
    glm::vec3 m_MinScale; 
    glm::vec3 m_MaxScale;
};

}
