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

    const v3& GetCurrentScale() const;
    const v3& GetGrowth() const;
    const v3& GetMinScale() const;
    const v3& GetMaxScale() const;
    
protected:
    v3 m_CurrentScale;
    v3 m_Growth; 
    v3 m_MinScale; 
    v3 m_MaxScale;
};

}
