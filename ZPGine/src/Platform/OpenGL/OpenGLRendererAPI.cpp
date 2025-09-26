#include "OpenGLRendererAPI.h"
#include "OpenGLCore.h"
#include <glad/glad.h>

namespace ZPG {

void OpenGLRendererAPI::Init() {
    ZPG_OPENGL_CALL(glEnable(GL_BLEND));
    ZPG_OPENGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void OpenGLRendererAPI::Shutdown() {

}

void OpenGLRendererAPI::SetClearColor(glm::vec4 color) {
    ZPG_OPENGL_CALL(glClearColor(color.r, color.g, color.b, color.a));
}

void OpenGLRendererAPI::Clear() {
    ZPG_OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height) {
    ZPG_CORE_DEBUG("{}", __PRETTY_FUNCTION__);
    ZPG_OPENGL_CALL(glViewport(x, y, width, height));
}

void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, const u32 indexCount) {
    int count = indexCount == 0 
                    ? vertexArray->GetIndexBuffer()->GetCount()
                    : indexCount;
    ZPG_OPENGL_CALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}

}
