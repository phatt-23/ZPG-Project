#pragma once

#include "Movement.h"

namespace ZPG
{
    class BezierMovement : public Movement
    {
    public:
        BezierMovement(const vec<v3>& controlPoints, float duration);
        virtual ~BezierMovement() override;
        void Update(Timestep &ts) override;
        
        const vec<v3>& GetControlPoints() const { return m_ControlPoints; }
        void SetControlPoints(const vec<v3>& controlPoints) { m_ControlPoints = controlPoints; }
        
    private:
        vec<v3> m_ControlPoints;
    };
}
