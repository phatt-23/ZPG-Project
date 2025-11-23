#pragma once

namespace ZPG 
{
    class Timestep;

    class Transform 
    {
    public:
        Transform();
        Transform(const m4& matrix);

        virtual ~Transform();

        /*
        * Gets the computed m_Matrix.
        * User must call either Update or ComputeMatrix to update/compute the m_Matrix.
        *
        * (should be const, but left un-const for flexibility - this shit was a mistake fuuuuuuuuuuuuuuck)
        */
        virtual const m4& GetMatrix();

        // Called in every iteration of OnUpdate() in a Scene
        virtual void Update([[maybe_unused]] Timestep& ts);

        virtual v3 GetScaleVector() const;
        virtual v3 GetPositionVector() const;

        // Computes the m_Matrix.
        virtual void ComputeMatrix() = 0;

    protected:
        m4 m_Matrix;
    };
}

