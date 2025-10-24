#include "Renderer/RendererAPI.h"

namespace ZPG {

class OpenGLRendererAPI : public RendererAPI {
public:
    void Init() override;
    void Shutdown() override;
    void SetClearColor(glm::vec4 color) override;
    void Clear() override;
    void SetViewport(int x, int y, int width, int height) override;
    void DrawIndexed(const VertexArray& vertexArray, const u32 indexCount) override;
    void DrawArrays(const VertexArray& vertexArray) override;
    void DrawIndexedInstanced(const VertexArray& vertexArray, const u32 indexCount, const u32 instanceCount) override;
    void DrawArraysInstanced(const VertexArray& vertexArray, const u32 instanceCount) override;
private:
};

}
