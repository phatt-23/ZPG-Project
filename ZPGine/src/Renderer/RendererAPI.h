//
// Created by phatt on 9/22/25.
//

#ifndef RENDERER_API_H
#define RENDERER_API_H

namespace ZPG {

class VertexArray;

class RendererAPI {
public:
    enum API { None, OpenGL, };

    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void SetClearColor(glm::vec4 color) = 0;
    virtual void Clear() = 0;
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    virtual void DrawIndexed(const VertexArray& vertexArray, const u32 indexCount) = 0;
    virtual void DrawArrays(const VertexArray& vertexArray) = 0;
    virtual void DrawIndexedInstanced(const VertexArray& vertexArray, const u32 indexCount, const u32 instanceCount) = 0;
    virtual void DrawArraysInstanced(const VertexArray& vertexArray, const u32 instanceCount) = 0;

    inline static API GetAPI() { return s_API; }
private:
    inline static API s_API = API::OpenGL;
};

}

#endif
