#pragma once

#include "Transform/MovementTransform/Movement.h"

namespace ZPG
{
    class LineMovement : public Movement
    {
    public: 
        LineMovement(v3 start, v3 end, float duration);
        virtual ~LineMovement() override;

        void Update(Timestep& ts) override;

        const v3& GetStartPoint() const { return m_StartPoint; }
        const v3& GetEndPoint() const { return m_EndPoint; }
        void SetStartPoint(const v3& startPoint) { m_StartPoint = startPoint; }
        void SetEndPoint(const v3& endPoint) { m_EndPoint = endPoint; }
    private:
        v3 m_StartPoint;
        v3 m_EndPoint;
    };
}
