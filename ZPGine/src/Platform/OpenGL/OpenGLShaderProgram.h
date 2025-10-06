#pragma once

#include "Shader/ShaderProgram.h"
#include <unordered_map>
#include "Shader/Shader.h"

namespace ZPG {

class OpenGLShaderProgram : public ShaderProgram {
public:
    OpenGLShaderProgram(const std::string& filepath);
    OpenGLShaderProgram(const std::string& name, const std::string& filepath);
    OpenGLShaderProgram(const std::string& name, const std::vector<Ref<Shader>>& shaders);
    ~OpenGLShaderProgram() override;

    virtual void Bind() override;
    virtual void Unbind() override;

    virtual void SetInt(const std::string& uniName, i32 scalar) override;
    virtual void SetInt2(const std::string& uniName, glm::i32vec2 vec) override;
    virtual void SetInt3(const std::string& uniName, glm::i32vec3 vec) override;
    virtual void SetInt4(const std::string& uniName, glm::i32vec4 vec) override;
    virtual void SetFloat(const std::string& uniName, f32 scalar) override;
    virtual void SetFloat2(const std::string& uniName, glm::f32vec2 vec) override;
    virtual void SetFloat3(const std::string& uniName, glm::f32vec3 vec) override;
    virtual void SetFloat4(const std::string& uniName, glm::f32vec4 vec) override;
    virtual void SetMat3(const std::string& uniName, glm::mat3 mat) override;
    virtual void SetMat4(const std::string& uniName, glm::mat4 mat) override;
private:
    i32 GetUniformLocation(const std::string& name);

    // Attaches provided shaders to this program and links it.
    static u32 LinkShaders(const std::vector<Ref<Shader>>& shaders);
    // Compiles all the shader sources and puts them into a list.
    std::vector<Ref<Shader>> CompileShaderSources(std::unordered_map<Shader::ShaderType, std::string> shaderSources);
    // Parses the input source code containing possibly many shader source codes into pairs of ShaderType and its source code.
    static std::unordered_map<Shader::ShaderType, std::string> GetShaderSources(const std::string& input);
private:
    u32 m_RendererID;
    std::string m_Name;
    std::vector<Ref<Shader>> m_Shaders;
    std::unordered_map<std::string, i32> m_LocationCache;  // identity map pattern
};

}
