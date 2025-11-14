#include "Buffer/VertexArray.h"
#include "Renderer/RenderPass.h"
namespace ZPG
{
    class ShaderProgram;
    class TextureCubeMap;
    class Texture2D;

    class LightingRenderPass : public RenderPass 
    {
    public:
        LightingRenderPass();
        ~LightingRenderPass();
        
        void Init(RenderContext &renderContext) override;
        void Execute(RenderContext &renderContext) override;
        
    private:
        ref<ShaderProgram> m_ShaderProgram;
        ref<TextureCubeMap> m_NullSkyboxCubeMap;
        ref<Texture2D> m_NullSkydomeMap;
        ref<VertexArray> m_QuadVAO;
    };
}
