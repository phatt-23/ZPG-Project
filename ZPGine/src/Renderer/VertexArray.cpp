#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace ZPG {

Ref<VertexArray> VertexArray::Create() {
    switch (Renderer::GetAPI()) {
        case RendererAPI::OpenGL: 
            return CreateRef<OpenGLVertexArray>();
        case RendererAPI::None: break;
    }
    ZPG_UNREACHABLE();
}
Ref<VertexArray> VertexArray::Create(const std::vector<Ref<VertexBuffer>>& vertexBuffers) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::OpenGL: 
            OpenGLVertexArray* VAO = new OpenGLVertexArray();
            for (auto& VBO : vertexBuffers) {
                VAO->AddVertexBuffer(VBO);
            }
            return CreateRef(VAO);
            break;
        case RendererAPI::None: 
            break;
    }
    ZPG_UNREACHABLE();
}
Ref<VertexArray> VertexArray::Create(const std::vector<Ref<VertexBuffer>>& vertexBuffers, const Ref<IndexBuffer>& indexBuffer) {
    switch (Renderer::GetAPI()) {
        case RendererAPI::OpenGL: 
            OpenGLVertexArray* VAO = new OpenGLVertexArray();
            for (auto& VBO : vertexBuffers) {
                VAO->AddVertexBuffer(VBO);
            }
            VAO->SetIndexBuffer(indexBuffer);
            return CreateRef(VAO);
            break;
        case RendererAPI::None: 
            break;
    }
    ZPG_UNREACHABLE();
}

}
