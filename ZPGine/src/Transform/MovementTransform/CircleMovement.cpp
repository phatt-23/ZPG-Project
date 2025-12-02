#include "CircleMovement.h"
#include "Core/Timestep.h"
#include "Core/Math.h"

namespace ZPG
{
    CircleMovement::CircleMovement(const v3& center, float radius, const v3& rotationAxis, float duration, MovementMode mode)
        : Movement(duration, mode)
        , m_Center(center)
        , m_Radius(radius)
        , m_RotationAxis(glm::normalize(rotationAxis))
    {
        // choose a up-vector
        v3 up = glm::abs(glm::dot(m_RotationAxis, v3(0,1,0))) > 0.999f
            ? v3(0,0,1)   // if direction and up are parallel
            : v3(0,1,0);

        qtr rotationQuaternion = glm::quatLookAt(glm::normalize(m_RotationAxis), up);
        m_RotationMatrix = glm::mat4_cast(rotationQuaternion);
    }

    CircleMovement::~CircleMovement()
    {
    }

    v3 CircleMovement::GetCurrentHeading() 
    {
        float alpha = GetCurrentAlpha();
        v3 heading = m_Radius * v3(-glm::sin(alpha), glm::cos(alpha), 0.0f);
        heading = v3(m_RotationMatrix * v4(heading, 1.0));
        return heading;
    }

    float CircleMovement::GetCurrentAlpha() const
    {
        float t = GetNormalizedTime();
        constexpr float TWO_PI = 2.0f * glm::pi<float>();
        float alpha = TWO_PI * t;
        return alpha;
    }

    v3 CircleMovement::GetCurrentPosition()
    {
        float alpha = GetCurrentAlpha();
        v3 position = m_Radius * v3(glm::cos(alpha), glm::sin(alpha), 0.0f);

        v3 currentPoint(v3(m_RotationMatrix * v4(position, 1.0f)) + m_Center);
        return currentPoint;
    }

    void CircleMovement::Update(Timestep& ts)
    {
        Movement::Update(ts);
        m4 R = Math::RotationMatFromDirection(GetCurrentHeading());
        m4 T = glm::translate(m4(1.0f), GetCurrentPosition());
        m_Matrix = T * R;
    }
}
