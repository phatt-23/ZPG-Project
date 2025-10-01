//
// Created by phatt on 9/22/25.
//

#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include "Renderer.h"
#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ZPG {

class RenderCommand {
public:
    inline static void Init() {
        ZPG_CORE_ASSERT(!s_RenderCommandInitialised, "RenderCommand was already initialized.");

        switch (Renderer::GetAPI()) {
        case RendererAPI::OpenGL: s_RendererAPI = new OpenGLRendererAPI(); break;
        case RendererAPI::None:
        default:
            ZPG_UNREACHABLE("RenderCommand couldn't instatiate RendererAPI. API not known.");
        }

        s_RendererAPI->Init();
    }

    static void Shutdown() {
        s_RendererAPI->Shutdown();
    }
    static void SetClearColor(glm::vec4 color) {
        s_RendererAPI->SetClearColor(color);
    }
    static void Clear() {
        s_RendererAPI->Clear();
    }
    static void SetViewport(int x, int y, int width, int height) {
        s_RendererAPI->SetViewport(x, y, width, height);
    }
    static void DrawIndexed(const Ref<VertexArray>& vertexArray, const u32 indexCount = 0) {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }
    static void DrawArrays(const Ref<VertexArray>& vertexArray) {
        s_RendererAPI->DrawArrays(vertexArray);
    }

private:
    inline static RendererAPI* s_RendererAPI = nullptr;
    inline static bool s_RenderCommandInitialised = false;
};

}

#endif //RENDERCOMMAND_H
