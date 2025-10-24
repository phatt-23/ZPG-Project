//
// Created by phatt on 10/18/25.
//
#include "RenderCommand.h"


#include "Renderer.h"
#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace ZPG {

void RenderCommand::Init() {
    ZPG_CORE_ASSERT(!s_RenderCommandInitialised, "RenderCommand was already initialized.");

    switch (Renderer::GetAPI()) {
    case RendererAPI::OpenGL: s_RendererAPI = new OpenGLRendererAPI(); break;
    case RendererAPI::None:
    default:
        ZPG_UNREACHABLE("RenderCommand couldn't instantiate RendererAPI. API not known.");
    }

    s_RendererAPI->Init();
}
void RenderCommand::Shutdown() {
    s_RendererAPI->Shutdown();
}
void RenderCommand::SetClearColor(v4 color) {
    s_RendererAPI->SetClearColor(color);
}
void RenderCommand::Clear() {
    s_RendererAPI->Clear();
}
void RenderCommand::SetViewport(int x, int y, int width, int height) {
    s_RendererAPI->SetViewport(x, y, width, height);
}
void RenderCommand::DrawIndexed(const VertexArray& vertexArray, const u32 indexCount) {
    s_RendererAPI->DrawIndexed(vertexArray, indexCount);
}
void RenderCommand::DrawArrays(const VertexArray& vertexArray) {
    s_RendererAPI->DrawArrays(vertexArray);
}

void RenderCommand::DrawIndexedInstanced(
    const VertexArray& vertexArray,
    const u32 indexCount,
    const u32 instanceCount
) {
    s_RendererAPI->DrawIndexedInstanced(vertexArray, indexCount, instanceCount);
}

void RenderCommand::DrawArraysInstanced(
    const VertexArray& vertexArray,
    const u32 instanceCount
) {
    s_RendererAPI->DrawArraysInstanced(vertexArray, instanceCount);
}

}
