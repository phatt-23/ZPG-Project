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
        ZPG_CORE_ASSERT(c1, "either transparent, or opaque");

        bool c2 = !((*this & RenderFeatureDeferred) && (*this & RenderFeatureTransparent));
        ZPG_CORE_ASSERT(c2, "if deferred, cannot be transparent");

        bool c3 = (*this & RenderFeatureForward) ^ (*this & RenderFeatureDeferred);
        ZPG_CORE_ASSERT(c3, "must be either forward or deferred");

        bool c4 = (*this & RenderFeatureStatic) ^ (*this & RenderFeatureDynamic);
        ZPG_CORE_ASSERT(c4, "must be either static or dynamic");

        return c1 && c2 && c3 && c4;
    }

} // ZPG