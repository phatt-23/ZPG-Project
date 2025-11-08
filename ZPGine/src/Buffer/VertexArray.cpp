#include "VertexArray.h"
#include "Renderer/Renderer.h"
#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ZPG {

ref<VertexArray> VertexArray::Create() {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::OpenGL: 
            return MakeRef<OpenGLVertexArray>();
        case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}
ref<VertexArray> VertexArray::Create(const std::vector<ref<VertexBuffer>>& vertexBuffers) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::OpenGL: {
            OpenGLVertexArray* VAO = new OpenGLVertexArray();
            for (auto& VBO : vertexBuffers) {
                VAO->AddVertexBuffer(VBO);
            }
            return MakeRef(VAO);
        } break;
        case RendererAPI::None: 
            break;
    }
    ZPG_UNREACHABLE();
}
ref<VertexArray> VertexArray::Create(const std::vector<ref<VertexBuffer>>& vertexBuffers, const ref<IndexBuffer>& indexBuffer) {
    switch (RendererAPI::GetAPI()) {
        case RendererAPI::OpenGL: {
            OpenGLVertexArray* VAO = new OpenGLVertexArray();
            for (auto& VBO : vertexBuffers) {
                VAO->AddVertexBuffer(VBO);
            }
            VAO->SetIndexBuffer(indexBuffer);
            return MakeRef(VAO);
        } break;
        case RendererAPI::None: 
            break;
    }
    ZPG_UNREACHABLE();
}

}
