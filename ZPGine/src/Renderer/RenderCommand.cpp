//
// Created by phatt on 10/18/25.
//
#include "RenderCommand.h"


#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

    void RenderCommand::Init() {
        ZPG_PROFILE_FUNCTION();
        ZPG_CORE_ASSERT(!s_RenderCommandInitialised, "RenderCommand was already initialized.");

        switch (RendererAPI::GetAPI()) {
        case RendererAPI::OpenGL: s_RendererAPI = new OpenGLRendererAPI(); break;
        case RendererAPI::None:
        default:
            ZPG_UNREACHABLE("RenderCommand couldn't instantiate RendererAPI. API not known.");
        }

        s_RendererAPI->Init();
    }
    void RenderCommand::Shutdown() {
        ZPG_PROFILE_FUNCTION();
        s_RendererAPI->Shutdown();
    }
    void RenderCommand::SetClearColor(v4 color) {
        ZPG_PROFILE_FUNCTION();
        s_RendererAPI->SetClearColor(color);
    }
    void RenderCommand::Clear() {
        ZPG_PROFILE_FUNCTION();
        s_RendererAPI->Clear();
    }
    void RenderCommand::SetViewport(int x, int y, int width, int height) {
        ZPG_PROFILE_FUNCTION();
        s_RendererAPI->SetViewport(x, y, width, height);
    }
    void RenderCommand::DrawIndexed(const VertexArray& vertexArray, const u32 indexCount) {
        ZPG_PROFILE_FUNCTION();
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }
    void RenderCommand::DrawArrays(const VertexArray& vertexArray) {
        ZPG_PROFILE_FUNCTION();
        s_RendererAPI->DrawArrays(vertexArray);
    }

    void RenderCommand::DrawIndexedInstanced(
        const VertexArray& vertexArray,
        const u32 indexCount,
        const u32 instanceCount
    ) {
        ZPG_PROFILE_FUNCTION();
        s_RendererAPI->DrawIndexedInstanced(vertexArray, indexCount, instanceCount);
    }

    void RenderCommand::DrawArraysInstanced(
        const VertexArray& vertexArray,
        const u32 instanceCount
    ) {
        ZPG_PROFILE_FUNCTION();
        s_RendererAPI->DrawArraysInstanced(vertexArray, instanceCount);
    }

}
