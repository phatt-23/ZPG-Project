#include "OpenGLRendererAPI.h"
#include "OpenGLCore.h"
#include <glad/glad.h>

namespace ZPG {

void OpenGLRendererAPI::Init() {
    ZPG_OPENGL_CALL(glEnable(GL_BLEND));
    ZPG_OPENGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    ZPG_OPENGL_CALL(glEnable(GL_DEPTH_TEST));   // Enable depth testing / z-buffer is big as the color buffer.
    // If the fragment's depth value is less than the stored depth value the fragment's color passes.
    // By default, this is set to GL_LESS.
    ZPG_OPENGL_CALL(glDepthFunc(GL_LESS));

    ZPG_OPENGL_CALL(glCullFace(GL_BACK));
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
    ZPG_OPENGL_CALL(glViewport(x, y, width, height));
}

void OpenGLRendererAPI::DrawIndexed(const VertexArray& vertexArray, const u32 indexCount) {
    int count = indexCount == 0 
                    ? vertexArray.GetIndexBuffer()->GetCount()
                    : indexCount;
    ZPG_OPENGL_CALL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
}
void OpenGLRendererAPI::DrawArrays(const VertexArray& vertexArray) {
    u64 count = 0;
    for (auto& vbo : vertexArray.GetVertexBuffers()) {
        count += vbo->GetSize() / vbo->GetLayout().GetStride();
    }

    ZPG_OPENGL_CALL(glDrawArrays(GL_TRIANGLES, 0, count));
}

}
