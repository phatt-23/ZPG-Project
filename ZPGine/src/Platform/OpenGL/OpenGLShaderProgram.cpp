#include "OpenGLShaderProgram.h"
#include "OpenGLCore.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Debug/Asserter.h"
#include "Core/Utility.h"
#include "Shader/Shader.h"

namespace ZPG {

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& filepath) {
    m_Name = Utility::GetNameFromPath(filepath);

    std::unordered_map<Shader::ShaderType, std::string> shaderSources = GetShaderSources(Utility::ReadFile(filepath));
    std::vector<ref<Shader>> shaders = CompileShaderSources(shaderSources);
    m_RendererID = LinkShaders(shaders);
}
OpenGLShaderProgram::OpenGLShaderProgram(const std::string& name, const std::string& filepath) : m_Name(name) {
    std::unordered_map<Shader::ShaderType, std::string> shaderSources = GetShaderSources(Utility::ReadFile(filepath));
    std::vector<ref<Shader>> shaders = CompileShaderSources(shaderSources);
    m_RendererID = LinkShaders(shaders);
}
OpenGLShaderProgram::OpenGLShaderProgram(const std::string& name, const std::vector<ref<Shader>>& shaders) {
    m_Name = name;
    m_RendererID = LinkShaders(shaders);
}
OpenGLShaderProgram::~OpenGLShaderProgram() {
    ZPG_OPENGL_CALL(glDeleteProgram(m_RendererID));
}
void OpenGLShaderProgram::Bind() {
    ZPG_OPENGL_CALL(glUseProgram(m_RendererID));
}
void OpenGLShaderProgram::Unbind() {
    ZPG_OPENGL_CALL(glUseProgram(0));
}
i32 OpenGLShaderProgram::GetUniformLocation(const std::string &name) {
    if (m_LocationCache.contains(name))
        return m_LocationCache[name];

    ZPG_OPENGL_CALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        ZPG_CORE_ERROR("Shader ({}, {}): Uniform with name '{}' wasn't found!", m_RendererID, m_Name, name);

    m_LocationCache[name] = location;
    return location;
}
void OpenGLShaderProgram::SetInt(const std::string& uniName, i32 scalar) {
    ZPG_OPENGL_CALL(glUniform1i(GetUniformLocation(uniName), scalar));
}
void OpenGLShaderProgram::SetInt2(const std::string& uniName, glm::i32vec2 vec) {
    ZPG_OPENGL_CALL(glUniform2iv(GetUniformLocation(uniName), 1, glm::value_ptr(vec)));
}
void OpenGLShaderProgram::SetInt3(const std::string& uniName, glm::i32vec3 vec) {
    ZPG_OPENGL_CALL(glUniform3iv(GetUniformLocation(uniName), 1, glm::value_ptr(vec)));
}
void OpenGLShaderProgram::SetInt4(const std::string& uniName, glm::i32vec4 vec) {
    ZPG_OPENGL_CALL(glUniform4iv(GetUniformLocation(uniName), 1, glm::value_ptr(vec)));
}
void OpenGLShaderProgram::SetFloat(const std::string& uniName, f32 scalar) {
    ZPG_OPENGL_CALL(glUniform1f(GetUniformLocation(uniName), scalar));
}
void OpenGLShaderProgram::SetFloat2(const std::string& uniName, glm::f32vec2 vec) {
    ZPG_OPENGL_CALL(glUniform2fv(GetUniformLocation(uniName), 1, glm::value_ptr(vec)));
}
void OpenGLShaderProgram::SetFloat3(const std::string& uniName, glm::f32vec3 vec) {
    ZPG_OPENGL_CALL(glUniform3fv(GetUniformLocation(uniName), 1, glm::value_ptr(vec)));
}
void OpenGLShaderProgram::SetFloat4(const std::string& uniName, glm::f32vec4 vec) {
    ZPG_OPENGL_CALL(glUniform4fv(GetUniformLocation(uniName), 1, glm::value_ptr(vec)));
}
void OpenGLShaderProgram::SetMat3(const std::string& uniName, glm::mat3 mat) {
    ZPG_OPENGL_CALL(glUniformMatrix3fv(GetUniformLocation(uniName), 1, GL_FALSE, glm::value_ptr(mat)));
}
void OpenGLShaderProgram::SetMat4(const std::string& uniName, glm::mat4 mat) {
    ZPG_OPENGL_CALL(glUniformMatrix4fv(GetUniformLocation(uniName), 1, GL_FALSE, glm::value_ptr(mat)));
}

u32 OpenGLShaderProgram::LinkShaders(const std::vector<ref<Shader>>& shaders) {
    int programID;
    ZPG_OPENGL_CALL(programID = glCreateProgram());
    ZPG_CORE_ASSERT(programID != 0, "Failed to create OpenGL program!");
    
    for (ref<Shader> shader : shaders) {
        shader->AttachTo(programID);
    }

    ZPG_OPENGL_CALL(glLinkProgram(programID));

    i32 status;
    ZPG_OPENGL_CALL(glGetProgramiv(programID, GL_LINK_STATUS, &status));
    if (status == GL_FALSE) {
        i32 len;
        ZPG_OPENGL_CALL(glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &len)); 
        
        char* msg = new char[len];
        ZPG_OPENGL_CALL(glGetProgramInfoLog(programID, len, &len, msg));
        ZPG_UNREACHABLE("ShaderProgram linking failed: {}", msg);

        ZPG_OPENGL_CALL(glDeleteProgram(programID));
        for (ref<Shader> shader : shaders) {
            shader->DetachFrom(programID);
        }

        delete[] msg;
        return -1;
    }
    return programID;
}
std::vector<ref<Shader>> OpenGLShaderProgram::CompileShaderSources(std::unordered_map<Shader::ShaderType, std::string> shaderSources) {
    std::vector<ref<Shader>> shaders;
    for (auto& [type, code] : shaderSources) {
        // default name will be the (current program's name).(vertex or fragment...)
        std::string shaderName = m_Name + "." + Shader::ShaderTypeToString(type);
        ref<Shader> shader = Shader::CreateFromCode(shaderName, type, code);
        shaders.push_back(shader);
    }
    return shaders;
}
std::unordered_map<Shader::ShaderType, std::string> OpenGLShaderProgram::GetShaderSources(const std::string& input) {
    using namespace std;

    unordered_map<Shader::ShaderType, std::string> shaders;
    const char* typeDirectiveTok = "#type";
    size_t p = 0;

    p = input.find(typeDirectiveTok, p);

    while (p != string::npos) {
        size_t lineEnd = min(input.find("\n", p), input.find("\r\n"));
        size_t afterTok = p + strlen(typeDirectiveTok);

        ZPG_CORE_ASSERT(afterTok < lineEnd, "After #type is nothing.")
        ZPG_CORE_ASSERT(input[afterTok] == ' ', "Missing space after #type.");

        size_t b = input.find_first_not_of(' ', afterTok);
        size_t e = min({ input.find(" ", b), input.find("//", b), lineEnd });
        const string& type = input.substr(b, e - b);

        Shader::ShaderType shaderType = Shader::ShaderTypeFromString(type);

        ZPG_CORE_ASSERT(shaderType != Shader::ShaderType::None, "Shader type found after #type not supported: {}", type);

        size_t nextLine = input.find_first_not_of("\r\n", lineEnd);
        size_t nextTok = input.find(typeDirectiveTok, nextLine);

        const string& shaderSource = input.substr(nextLine, (nextTok == string::npos ? input.size() : nextTok) - nextLine);
        shaders[shaderType] = shaderSource;

        p = input.find(typeDirectiveTok, nextLine);
    }

    ZPG_CORE_ASSERT(!shaders.empty(), "There is no #type specification in the source.");
    return shaders;
}



// // deperecated, compiles the shader sources, links a program and deletes the compiled shaders.
// u32 Compile(std::unordered_map<Shader::ShaderType, std::string> sources) {
//     u32 program;
//     ZPG_OPENGL_CALL(program = glCreateProgram());

//     std::vector<u32> shaders(sources.size());
//     for (auto& [type, source] : sources) {
//         u32 shaderId = OpenGLShader::CompileShader(type, source);
//         shaders.push_back(shaderId);
//         ZPG_OPENGL_CALL(glAttachShader(program, shaderId));
//     }

//     ZPG_OPENGL_CALL(glLinkProgram(program));

//     i32 status;
//     ZPG_OPENGL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &status));
//     if (status == GL_FALSE) {
//         i32 len;
//         ZPG_OPENGL_CALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len)); 
        
//         char* msg = new char[len];
//         ZPG_OPENGL_CALL(glGetProgramInfoLog(program, len, &len, msg));

//         ZPG_OPENGL_CALL(glDeleteProgram(program));
//         for (u32 shader : shaders) {
//             ZPG_OPENGL_CALL(glDeleteShader(shader));
//         }

//         ZPG_UNREACHABLE("ShaderProgram linking failed: {}", msg);

//         delete[] msg;
//         return 0;
//     }
    
//     for (u32 shId : shaders) {
//         ZPG_OPENGL_CALL(glDeleteShader(shId));
//     }

//     return program;
// }





}

