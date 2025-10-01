//
// Created by phatt on 9/22/25.
//

#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include <glm/glm.hpp>
#include "Renderer/VertexArray.h"
#include "Core/Core.h"

namespace ZPG {

class RendererAPI {
public:
    enum API { None, OpenGL, };

    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void SetClearColor(glm::vec4 color) = 0;
    virtual void Clear() = 0;
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, const u32 indexCount = 0) = 0;
    virtual void DrawArrays(const Ref<VertexArray>& vertexArray) = 0;

    inline static API GetAPI() { return s_API; }
private:
    inline static API s_API = API::OpenGL;
};

}

#endif //RENDERERAPI_H
