#include "CircleMovement.h"
#include "Core/Timestep.h"

namespace ZPG
{
    CircleMovement::CircleMovement(const v3& center, float radius, const v3& direction, float duration, MovementMode mode)
        : Movement(duration, mode)
        , m_Center(center)
        , m_Radius(radius)
        , m_Direction(glm::normalize(direction))
    {
    }

    CircleMovement::~CircleMovement()
    {
    }

    v3 CircleMovement::GetCurrentPosition()
    {
        constexpr float TWO_PI = 2.0f * glm::pi<float>();
        float alpha = TWO_PI * (m_CurrentTime / m_Duration);
        v3 position = m_Radius * v3(glm::cos(alpha), glm::sin(alpha), 0.0f);

        // choose a up-vector
        v3 up = glm::abs(glm::dot(m_Direction, v3(0,1,0))) > 0.999f
            ? v3(0,0,1)   // if direction and up are parallel
            : v3(0,1,0);

        qtr rotationQuaternion = glm::quatLookAt(glm::normalize(m_Direction), up);
        m4 R = glm::mat4_cast(rotationQuaternion);

        v3 currentPoint(v3(R * v4(position, 1.0f)) + m_Center);
        return currentPoint;
    }

    void CircleMovement::Update(Timestep& ts)
    {
        Movement::Update(ts);
        v3 currentPoint = GetCurrentPosition();
        m_Matrix = glm::translate(m4(1.0f), currentPoint);
    }
}
