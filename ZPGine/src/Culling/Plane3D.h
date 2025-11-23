#pragma once


namespace ZPG
{
    enum class Halfspace
    {
        Front,      // point is on the side the normal points to
        Back,       // opposite side
        OnPlane,    // point lies on the plane
    };

    /*
    * Expressed by the equation ax + by + cz + d. 
    * Contains a normal = (a,b,c) and coeficient d
    * A plane slices the 3D space into 2 sides: 
    *   - positive half space (the space where the normal is pointing towards)
    *   - negative half space (the space on the opposite side)
    * I decide to call them:
    *   - front half space
    *   - back half space
    */
    class Plane3D
    {
    public:
        Plane3D();

        /*
        * Constructs a plane out of 3 points in space.
        *
        * Correctness was proven via Desmos visualisation.
        */
        Plane3D(const v3& p0, const v3& p1, const v3& p2);
                   
        /*
        * Calculates the distance between the plane and the `point`.
        * If positive, then point is infront of the surface. 
        * If negative, then point is behind the surface
        *
        * Correctness was proven via Desmos visualisation.
        */
        float SignedDistance(const v3& point) const;

        Halfspace ClassifyPoint(const v3& point) const;

        bool IsInFrontHalfspace(const v3& point) const;
        bool IsInBackHalfspace(const v3& point) const;
        bool IsOnPlane(const v3& point) const;
    
    private:
        v3 m_Normal; 
        float m_D;
    };
}

