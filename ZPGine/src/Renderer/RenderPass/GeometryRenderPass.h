#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class Texture2D;

    /**
     * Populates the geometry framebuffer with informations about 
     * positions, normals, albedo, metallic, roughness, emissive
     * entityID and so on.
    */
    class GeometryRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };
}
