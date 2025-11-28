#pragma once

#include "Transform/MovementTransform/Movement.h"

namespace ZPG
{
    /**
     * Represents cubic Bezier curve.
     */
    class SplineSegment
    {
    public:
        SplineSegment(const v3& P1, const v3& P2, const v3& P3);
        SplineSegment(const v3& P0, const v3& P1, const v3& P2, const v3& P3);

        const std::array<v3, 4>& GetControlPoints() const;
        f32 GetLength();
        const v3& GetStartPoint() const;
        const v3& GetEndPoint() const;

        void SetStartPoint(const v3& startPoint);
        bool IsPointPresent(int i) const;

    private:
        u32 m_PresentPoints = 0;
        std::array<v3, 4> m_ControlPoints;
        f32 m_Length = -1;
    };

    /**
     * Spline using cubic Bezier curves.
    */
    class SplineMovement : public Movement
    {
    public:
        SplineMovement(const v3& startPoint, 
                       std::initializer_list<SplineSegment>&& splineSegments, 
                       float duration, 
                       MovementMode mode = MovementMode::Repeat);

        SplineMovement(const v3& startPoint, 
                       const std::vector<SplineSegment>& splineSegments, 
                       float duration, 
                       MovementMode mode = MovementMode::Repeat);

        void Invalidate();

        v3 GetCurrentPosition() override;
        v3 GetCurrentHeading() override;

        void Update(Timestep& ts) override;
    private:

        struct CurrentParameters
        {
            v3 P0, P1, P2, P3;
            float t;
        };

        CurrentParameters GetCurrentControlPoints();

    private:
        v3 m_StartPoint;
        std::vector<SplineSegment> m_SplineSegments;
        std::vector<float> m_SplineSegmentLengths;
        float m_TotalLength = -1;
    };
}
