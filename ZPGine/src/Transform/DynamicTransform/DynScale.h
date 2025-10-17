#pragma once

#include "Transform/DynTransform.h"

namespace ZPG {

class Timestep;

class DynScale : public DynamicTransform {
public:
    DynScale(
        const v3& startingScale,
        const v3& growth,
        const v3& minScale,
        const v3& maxScale);

    virtual void Update(Timestep& ts) override;

    void ComputeMatrix() override;
    
protected:
    glm::vec3 m_CurrentScale;
    glm::vec3 m_Growth; 
    glm::vec3 m_MinScale; 
    glm::vec3 m_MaxScale;
};

}
