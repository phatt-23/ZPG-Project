#include "Renderer/RenderPass.h"

namespace ZPG
{
    class ShaderProgram;

    class GeometryRenderPass : public RenderPass
    {
    public:
        GeometryRenderPass();
        ~GeometryRenderPass() override;

        void Init(RenderContext& renderContext) override;

        void Execute(RenderContext& renderContext) override;

        void Flush(RenderContext& renderContext);

    private:
        ref<ShaderProgram> m_ShaderProgram;
    };
}
