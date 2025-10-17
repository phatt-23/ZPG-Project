//
// Created by phatt on 9/22/25.
//

#ifndef RENDER_COMMAND_H
#define RENDER_COMMAND_H

namespace ZPG {

class VertexArray;
class RendererAPI;

class RenderCommand {
public:
    static void Init();
    static void Shutdown();
    static void SetClearColor(glm::vec4 color);
    static void Clear();
    static void SetViewport(int x, int y, int width, int height);
    static void DrawIndexed(const VertexArray& vertexArray, const u32 indexCount = 0);
    static void DrawArrays(const VertexArray& vertexArray);

private:
    inline static RendererAPI* s_RendererAPI = nullptr;
    inline static bool s_RenderCommandInitialised = false;
};

}

#endif
