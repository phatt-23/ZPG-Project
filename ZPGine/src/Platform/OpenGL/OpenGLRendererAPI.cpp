#include "OpenGLRendererAPI.h"
#include "OpenGLCore.h"
#include <glad/glad.h>

#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

    void OpenGLRendererAPI::Init() {
        ZPG_PROFILE_FUNCTION();
        // ZPG_OPENGL_CALL(glEnable(GL_BLEND));
        // ZPG_OPENGL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        ZPG_OPENGL_CALL(glEnable(GL_DEPTH_TEST));   // Enable depth testing / z-buffer is big as the color buffer.
        // If the fragment's depth value is less than the stored depth value the fragment's color passes.
        // By default, this is set to GL_LESS.
        ZPG_OPENGL_CALL(glDepthFunc(GL_LESS));

        // ZPG_OPENGL_CALL(glCullFace(GL_BACK));
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK); // default, culls back faces
        // glCullFace(GL_FRONT); // culls
    }

    void OpenGLRendererAPI::Shutdown() {
    }

    void OpenGLRendererAPI::SetClearColor(glm::vec4 color) {
        ZPG_PROFILE_FUNCTION();
        ZPG_OPENGL_CALL(glClearColor(color.r, color.g, color.b, color.a));
    }

    void OpenGLRendererAPI::Clear() {
        ZPG_PROFILE_FUNCTION();
        ZPG_OPENGL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height) {
        ZPG_PROFILE_FUNCTION();
        ZPG_OPENGL_CALL(glViewport(x, y, width, height));
    }

    void OpenGLRendererAPI::DrawIndexed(const VertexArray& vertexArray, const u32 indexCount) {
        ZPG_PROFILE_FUNCTION();
        vertexArray.Bind();
        ZPG_OPENGL_CALL(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr));
        vertexArray.Unbind();
    }
    void OpenGLRendererAPI::DrawArrays(const VertexArray& vertexArray) {
        ZPG_PROFILE_FUNCTION();
        vertexArray.Bind();
        ZPG_OPENGL_CALL(glDrawArrays(GL_TRIANGLES, 0, vertexArray.GetVertexCount()));
        vertexArray.Unbind();
    }

    void OpenGLRendererAPI::DrawIndexedInstanced(const VertexArray& vertexArray, const u32 indexCount, const u32 instanceCount) 
    {
        ZPG_PROFILE_FUNCTION();
        vertexArray.Bind();
        ZPG_OPENGL_CALL(glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr, instanceCount));
        vertexArray.Unbind();
    }

    void OpenGLRendererAPI::DrawArraysInstanced(const VertexArray& vertexArray, const u32 instanceCount) 
    {
        ZPG_PROFILE_FUNCTION();
        vertexArray.Bind();
        ZPG_OPENGL_CALL(glDrawArraysInstanced(GL_TRIANGLES, 0, vertexArray.GetVertexCount(), instanceCount));
        vertexArray.Unbind();
    }
}
