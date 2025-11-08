#include "Buffer/VertexArray.h"
#include "Renderer/RenderPass.h"
namespace ZPG
{
    class ShaderProgram;
    class CubemapTexture;
    class Texture;

    class LightingRenderPass : public RenderPass 
    {
    public:
        LightingRenderPass();
        ~LightingRenderPass();
        
        void Init(RenderContext &renderContext) override;
        void Execute(RenderContext &renderContext) override;
        
    private:
        ref<ShaderProgram> m_ShaderProgram;
        ref<CubemapTexture> m_NullSkyboxCubeMap;
        ref<Texture> m_NullSkydomeMap;
        ref<VertexArray> m_QuadVAO;
    };
}
