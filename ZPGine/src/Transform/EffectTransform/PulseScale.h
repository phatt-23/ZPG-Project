#pragma once
#include "Transform/DynamicTransform/DynScale.h"

namespace ZPG {

class PulseScale : public DynScale {
public:
    PulseScale(
        const glm::vec3& startingScale, 
        const glm::vec3& growth, 
        const glm::vec3& minScale, 
        const glm::vec3& maxScale);
    void Update(Timestep ts) override;
private:
};


}
