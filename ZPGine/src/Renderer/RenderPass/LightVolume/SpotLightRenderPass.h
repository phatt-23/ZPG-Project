#pragma once

#include "LightRenderPass.h"

namespace ZPG
{
    class SpotLightRenderPass : public LightRenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
};
}
