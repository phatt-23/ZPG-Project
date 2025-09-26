#pragma once

#include "Renderer/ShaderProgram.h"
#include <unordered_map>

namespace ZPG {

class OpenGLShaderProgram : public ShaderProgram {
public:
    OpenGLShaderProgram(const std::string& path);
    OpenGLShaderProgram(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
    ~OpenGLShaderProgram() override;

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void SetInt(const std::string& uniName, glm::i32 scalar) override;
    virtual void SetInt2(const std::string& uniName, glm::i32vec2 vec) override;
    virtual void SetInt3(const std::string& uniName, glm::i32vec3 vec) override;
    virtual void SetInt4(const std::string& uniName, glm::i32vec4 vec) override;
    virtual void SetFloat(const std::string& uniName, glm::f32 scalar) override;
    virtual void SetFloat2(const std::string& uniName, glm::f32vec2 vec) override;
    virtual void SetFloat3(const std::string& uniName, glm::f32vec3 vec) override;
    virtual void SetFloat4(const std::string& uniName, glm::f32vec4 vec) override;
    virtual void SetMat3(const std::string& uniName, glm::mat3 mat) override;
    virtual void SetMat4(const std::string& uniName, glm::mat4 mat) override;
private:
    std::unordered_map<u32, std::string> PreProcess(const std::string& input);
    u32 Compile(std::unordered_map<u32, std::string> sources);
    u32 CompileShader(u32 type, const std::string& source);
    i32 GetUniformLocation(const std::string& name);
private:
    u32 m_RendererID;
    std::string m_Name;
    std::unordered_map<std::string, i32> m_LocationCache;
};

}
