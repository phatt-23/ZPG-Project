#pragma once
#include "Renderer/Shader.h"
#include <glad/glad.h>

namespace ZPG {

class OpenGLShader : public Shader {
public:
    OpenGLShader(const std::string& filepath);
    OpenGLShader(const std::string& name, const std::string& filepath);
    OpenGLShader(const std::string& name, ShaderType type, const std::string& sourceCode);
    ~OpenGLShader() override;

    virtual ShaderType GetShaderType() const override;

    virtual void AttachTo(u32 shaderProgramID) const override;
    virtual void DetachFrom(u32 shaderProgramID) const override;
    
    static u32 CompileShader(ShaderType shaderType, const std::string& source);
    static std::pair<ShaderType, std::string> EatAndSetShaderTypeDirective(const std::string& source);
    static GLenum MapShaderTypeToOpenGLenum(ShaderType type);
    private:
private:
    ShaderType m_ShaderType;
    std::string m_Name;
    u32 m_RendererID = -1;
};

}
