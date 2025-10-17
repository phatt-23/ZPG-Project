#pragma once
#include "Transform/DynamicTransform/DynScale.h"

namespace ZPG {

class PulseScale : public DynScale {
public:
    PulseScale(
        const v3& startingScale,
        const v3& growth,
        const v3& minScale,
        const v3& maxScale);
    void Update(Timestep& ts) override;
private:
};


}
