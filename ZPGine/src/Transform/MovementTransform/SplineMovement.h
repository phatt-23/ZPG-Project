#pragma once

#include "Transform/MovementTransform/Movement.h"

namespace ZPG
{
    /*
     * Represents cubic Bezier curve.
     */
    class SplineSegment
    {
    public:
        /*
         * Fully defines of the cubic bezier.
         */
        SplineSegment(const v3& P0, const v3& P1, const v3& P2, const v3& P3);

        /*
         * The P3 of the previous spline segment is used as P0.
         * It's continuous, but it may contain spinodes (or cusps, harsh breaks).
         */
        SplineSegment(const v3& P1, const v3& P2, const v3& P3);

        /*
         * The P0 is the P3 of the previous segment.
         * The P1 is calculated from P2 and P3 of the previous segment.
         * It's continuous and without spinodes (smooth).
         */
        SplineSegment(const v3& P2, const v3& P3);

        const std::array<v3, 4>& GetControlPoints() const;
        const v3& GetStartPoint() const;
        const v3& GetControlPoint1() const;
        const v3& GetControlPoint2() const;
        const v3& GetEndPoint() const;
        f32 GetLength();

        void SetStartPoint(const v3& point);
        void SetControlPoint1(const v3& point);
       
        bool IsPointPresent(int i) const;
        void SetPoint(int index, const v3& point);

        f32 ComputeTFromDistance(f32 distance, int samples = 32);
        f32 ComputeLengthAccurate(int samples = 16) const;

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

        struct Bezier3Parameters
        {
            v3 P0, P1, P2, P3;
            float t;
        };

        Bezier3Parameters GetCurrentBezier3Parameters();

    private:
        v3 m_StartPoint;
        std::vector<SplineSegment> m_Segments;
        std::vector<float> m_SegmentLengths;
        float m_TotalLength = -1;
    };
}
