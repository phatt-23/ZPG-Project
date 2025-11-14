#pragma once

#include "Shader/Shader.h"

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
    
private:
    u32 CompileShader(ShaderType shaderType, const std::string& source);
    std::pair<ShaderType, std::string> EatAndSetShaderTypeDirective(const std::string& source);
    GLenum MapShaderTypeToOpenGLenum(ShaderType type);
    std::string ResolveIncludes(const std::string& source, const std::string& directoryPath, const std::string& filename);

private:
    ShaderType m_ShaderType;
    std::string m_Name;
    u32 m_RendererID = -1;
};

}
