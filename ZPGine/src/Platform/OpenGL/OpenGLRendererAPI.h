#include "Renderer/RendererAPI.h"

namespace ZPG {

class OpenGLRendererAPI : public RendererAPI {
public:
    virtual void Init() override;
    virtual void Shutdown() override;
    virtual void SetClearColor(glm::vec4 color) override;
    virtual void Clear() override;
    virtual void SetViewport(int x, int y, int width, int height) override;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const u32 indexCount = 0) override;
private:
};

}
