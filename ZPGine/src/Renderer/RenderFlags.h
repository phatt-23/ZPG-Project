//
// Created by phatt on 11/17/25.
//

#pragma once

namespace ZPG
{
    enum RenderFeature : u32
    {
        RenderFeatureCastsShadow = ZPG_BIT(1),
        RenderFeatureForward     = ZPG_BIT(2),
        RenderFeatureDeferred    = ZPG_BIT(3),
        RenderFeatureOpaque      = ZPG_BIT(4),
        RenderFeatureTransparent = RenderFeatureForward | ZPG_BIT(5),
        RenderFeatureStatic      = ZPG_BIT(6),
        RenderFeatureDynamic     = ZPG_BIT(7),
        RenderFeatureUseCustomShaderProgram = ZPG_BIT(8),
    };

    class RenderFlags
    {
    public:
        u32 Flags = 0;

        RenderFlags() = default;
        RenderFlags(u32 flags) : Flags(flags) {}
        RenderFlags(RenderFeature feature) : Flags((u32)feature) {}

        bool IsValid();

        bool IsSet(RenderFeature feature) { return Flags & (u32)feature; }
        bool operator==(RenderFlags other) { return Flags == other.Flags; }
        bool operator&(RenderFeature feature) { return IsSet(feature); }
        RenderFlags& operator|=(u32 flags) { Flags |= flags; return *this; }
        RenderFlags& operator|=(RenderFeature feature) { Flags |= (u32)feature; return *this; }
    };

}
