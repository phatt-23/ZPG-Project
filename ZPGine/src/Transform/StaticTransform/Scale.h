#pragma once

#include "Transform/StaticTransform.h"

namespace ZPG {

class Scale : public StaticTransform {
public:
    Scale(f32 scalar);
    Scale(f32 xScale, f32 yScale, f32 zScale);
    Scale(const v3& scale);
    void ComputeMatrix() override;

    const v3& GetScale() const;
    void SetScale(const v3& scale);
private:
    v3 m_ScaleVector;
};

}
