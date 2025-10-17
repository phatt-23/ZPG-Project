//
// Created by phatt on 9/22/25.
//

#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace ZPG {

class OpenGLContext : public GraphicsContext {
public:
    OpenGLContext(GLFWwindow* windowHandle);
    virtual ~OpenGLContext() override;

    virtual void Init() override;
    virtual void SwapBuffers() override;
private:
    GLFWwindow* m_WindowHandle;
};

}

#endif //OPENGLCONTEXT_H
