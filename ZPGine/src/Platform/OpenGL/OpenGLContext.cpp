#include "OpenGLContext.h"
#include "Debug/Asserter.h"
#include "OpenGLCore.h"

namespace ZPG {

OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
: m_WindowHandle(windowHandle) {
    ZPG_CORE_ASSERT(m_WindowHandle != nullptr);
}

OpenGLContext::~OpenGLContext() {

}

void OpenGLContext::Init() {

    // makes the OpenGL calls act upon this window
    // glfw function is here because it's OpenGL specific
    glfwMakeContextCurrent(m_WindowHandle);

    // load OpenGL extensions to the current window context
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    ZPG_CORE_ASSERT(status, "GLAD failed to init.");

    // log some stats
    i32 fragTextureMax, combinedTextureMax;
    ZPG_OPENGL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &fragTextureMax));
    ZPG_OPENGL_CALL(glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &combinedTextureMax));

    int versionMajor, versionMinor;
    ZPG_OPENGL_CALL(glGetIntegerv(GL_MAJOR_VERSION, &versionMajor));
    ZPG_OPENGL_CALL(glGetIntegerv(GL_MINOR_VERSION, &versionMinor));

    ZPG_CORE_INFO("OpenGL Info:");
    ZPG_CORE_INFO("  Vendor: {}", (char*)glGetString(GL_VENDOR));
    ZPG_CORE_INFO("  Renderer: {}", (char*)glGetString(GL_RENDERER));
    ZPG_CORE_INFO("  Version: {}", (char*)glGetString(GL_VERSION));
    ZPG_CORE_INFO("  Fragment shader texture binding maximum: {}", fragTextureMax);
    ZPG_CORE_INFO("  Maximum texture count that can be used: {}", combinedTextureMax);

    ZPG_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), 
                   "Engine requires OpenGL version of at least 4.5.");
}

void OpenGLContext::SwapBuffers() {
    // glfw function is here because it's OpenGL specific
    glfwSwapBuffers(m_WindowHandle);  
}

}

