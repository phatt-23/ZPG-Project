//
// Created by phatt on 11/17/25.
//

#include "RenderFlags.h"

#include "Debug/Asserter.h"

namespace ZPG
{

    bool RenderFlags::IsValid()
    {
        bool c1 = !((*this & RenderFeatureTransparent) && (*this & RenderFeatureOpaque));
        ZPG_CORE_ASSERT(c1, "Either transparent, or opaque.");

        bool c2 = !((*this & RenderFeatureDeferred) && (*this & RenderFeatureTransparent));
        ZPG_CORE_ASSERT(c2, "If deferred, cannot be transparent.");

        bool c3 = (*this & RenderFeatureForward) ^ (*this & RenderFeatureDeferred);
        ZPG_CORE_ASSERT(c3, "Must be either forward or deferred. Not both.");

        bool c4 = (*this & RenderFeatureStatic) ^ (*this & RenderFeatureDynamic);
        ZPG_CORE_ASSERT(c4, "Must be either static or dynamic. Not both.");

        if (*this & RenderFeatureUseCustomShaderProgram) {
            ZPG_CORE_ASSERT((*this & RenderFeatureForward), "When using a custom shader program, it must use forward rendering.");
        }


        return c1 && c2 && c3 && c4;
    }

} // ZPG
