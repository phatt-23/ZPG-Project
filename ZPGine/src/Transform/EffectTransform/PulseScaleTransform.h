#pragma once
#include "Transform/DynamicTransform/DynScaleTransform.h"

namespace ZPG {

class PulseScaleTransform : public DynScaleTransform {
public:
    PulseScaleTransform(
        const glm::vec3& startingScale, 
        const glm::vec3& growth, 
        const glm::vec3& minScale, 
        const glm::vec3& maxScale);
    void Update(Timestep ts) override;
private:
};


}
