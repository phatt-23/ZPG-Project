#pragma once

#include "Movement.h"

namespace ZPG
{

    class CircleMovement : public Movement
    {
    public:
        CircleMovement(const v3& center, float radius, const v3& direction, float duration, MovementMode mode = MovementMode::Once);
        virtual ~CircleMovement() override;
        
        void Update(Timestep& ts) override;

        v3 GetCurrentPosition() override;

        float GetRadius() const { return m_Radius; }
        const v3& GetCenter() const { return m_Center; }
        const v3& GetDirection() const { return m_Direction; }
        void SetRadius(float radius) { m_Radius = radius; }
        void SetCenter(const v3& center) { m_Center = center; }
        void SetDirection(const v3& direction) { m_Direction = normalize(direction); }
        
    private:
        float m_Radius;
        v3 m_Center;
        v3 m_Direction;
    };
}
