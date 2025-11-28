//
// Created by phatt on 11/28/25.
//
#pragma once
#include "Core.h"

namespace ZPG
{
    namespace Math
    {
        inline v3 Bezier3(const v3& P0, const v3& P1, const v3& P2, const v3& P3, float t)
        {
            float mt = 1 - t;
            return ( powf(mt, 3) * P0 )
                 + ( 3 * powf(mt, 2) * t * P1 )
                 + ( 3 * mt * powf(t, 2) * P2 )
                 + ( powf(t, 3) * P3 );
        }

        inline v3 TangentBezier3(const v3& P0, const v3& P1, const v3& P2, const v3& P3, float t)
        {
            float mt = 1 - t;
            return ( 3 * mt * mt   * (P1 - P0) )
                 + ( 6 * mt * t    * (P2 - P1) )
                 + ( 3 * t  * t    * (P3 - P2) );
        }

        inline m4 RotationMatFromDirection(const v3& direction)
        {
            v3 front = glm::normalize(direction);
            v3 worldUp = v3(0,1,0);

            if (glm::abs(glm::dot(front, worldUp)) > 0.999f)
            {
                worldUp = v3(1,0,0);
            }

            v3 right = glm::normalize(glm::cross(worldUp, front));
            v3 up = glm::cross(front, right);

            // object is aligned by these basis vectors
            m4 R(1.0f);
            R[0] = v4(right, 0.0f);
            R[1] = v4(up, 0.0f);
            R[2] = v4(front, 0.0f);

            return R;
        }
    }
}
