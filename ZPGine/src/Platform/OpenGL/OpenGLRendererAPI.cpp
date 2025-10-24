#include "OpenGLRendererAPI.h"
#include "OpenGLCore.h"
#include <glad/glad.h>

#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"

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
    ZPG_OPENGL_CALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
}
void OpenGLRendererAPI::DrawArrays(const VertexArray& vertexArray) {
    ZPG_OPENGL_CALL(glDrawArrays(GL_TRIANGLES, 0, vertexArray.GetVertexCount()));
}

void OpenGLRendererAPI::DrawIndexedInstanced(
    const VertexArray& vertexArray,
    const u32 indexCount,
    const u32 instanceCount
) {
    ZPG_OPENGL_CALL(glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr, instanceCount));
}

void OpenGLRendererAPI::DrawArraysInstanced(
    const VertexArray& vertexArray,
    const u32 instanceCount
) {
    ZPG_OPENGL_CALL(glDrawArraysInstanced(GL_TRIANGLES, 0, vertexArray.GetVertexCount(), instanceCount));
}
}
