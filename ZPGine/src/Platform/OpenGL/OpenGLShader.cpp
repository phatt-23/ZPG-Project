#include "OpenGLShader.h"
#include "OpenGLCore.h"
#include "Core/Utility.h"
#include <glad/glad.h>
#include <stb_include/stb_include.h>

#include "Profiling/Instrumentor.h"

namespace ZPG {

static std::string s_ShaderDirectoryPath = "./assets/shaders";

OpenGLShader::OpenGLShader(const std::string& filepath) {
    ZPG_PROFILE_FUNCTION();
    m_Name = Utility::GetNameFromPath(filepath);
    auto [shaderType, sourceCode] = EatAndSetShaderTypeDirective(Utility::ReadFile(filepath));
    m_ShaderType = shaderType;
    m_RendererID = CompileShader(shaderType, ResolveIncludes(sourceCode, s_ShaderDirectoryPath, filepath));
}

OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath) {
    ZPG_PROFILE_FUNCTION();
    m_Name = name;
    auto [shaderType, sourceCode] = EatAndSetShaderTypeDirective(Utility::ReadFile(filepath));
    m_ShaderType = shaderType;
    m_RendererID = CompileShader(shaderType, ResolveIncludes(sourceCode, s_ShaderDirectoryPath, filepath));
}

OpenGLShader::OpenGLShader(const std::string& name, ShaderType shaderType, const std::string& sourceCode) {
    ZPG_PROFILE_FUNCTION();
    m_Name = name;
    m_ShaderType = shaderType;
    m_RendererID = CompileShader(shaderType, ResolveIncludes(sourceCode, s_ShaderDirectoryPath, name));
}

OpenGLShader::~OpenGLShader() {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glDeleteShader(m_RendererID));
}

Shader::ShaderType OpenGLShader::GetShaderType() const {
    ZPG_PROFILE_FUNCTION();
    return m_ShaderType;
}

void OpenGLShader::AttachTo(u32 shaderProgramID) const {
    ZPG_PROFILE_FUNCTION();
    ZPG_OPENGL_CALL(glAttachShader(shaderProgramID, m_RendererID));
}

void OpenGLShader::DetachFrom(u32 shaderProgramID) const {
    ZPG_OPENGL_CALL(glDetachShader(shaderProgramID, m_RendererID));
}

GLenum OpenGLShader::MapShaderTypeToOpenGLenum(Shader::ShaderType type) {
    ZPG_PROFILE_FUNCTION();
    switch (type) {
        case ShaderType::Vertex: return GL_VERTEX_SHADER;
        case ShaderType::Fragment: return GL_FRAGMENT_SHADER;
        case ShaderType::Geometry: return GL_GEOMETRY_SHADER;
        default:
            ZPG_UNREACHABLE("Unknown shader type");
    }
    return -1;
}

std::string OpenGLShader::ResolveIncludes(const std::string& source, const std::string& directoryPath, const std::string& filename) {
    // Do include-processing on the string 'str'. To free the return value, pass it to free()
    // char *stb_include_string(char *str, char *inject, char *path_to_includes, char *filename_for_line_directive, char error[256]);

    std::string e;
    e.resize(256);

    std::string s(source);
    std::string d(directoryPath);
    std::string f(filename);

    char* res = stb_include_string(s.data(), nullptr, d.data(), f.data(), e.data());

    ZPG_CORE_ASSERT(res != nullptr, "Preprocessing of the shader {} in directory {} failed: {}", f.c_str(), d.c_str(), e.c_str());

    std::string result(res);

    free(res);

    return result;
}

std::pair<Shader::ShaderType, std::string> OpenGLShader::EatAndSetShaderTypeDirective(const std::string& source) {
    ZPG_PROFILE_FUNCTION();
    const char* typeDirectiveTok = "#type";
   
    size_t p = 0;
    p = source.find(typeDirectiveTok, p);

    ZPG_ASSERT(p != std::string::npos, "Every shader program must have a #type directive specified.");

    size_t lineEnd = source.find("\n", p);
    size_t afterTypeDirective = p + strlen(typeDirectiveTok);
    ZPG_ASSERT(afterTypeDirective < lineEnd);

    size_t b = source.find_first_not_of(' ', afterTypeDirective);
    size_t e = std::min({ source.find(" ", b), source.find("//", b), lineEnd });

    const std::string& type = source.substr(b, e - b);

    ShaderType shaderType = Shader::ShaderTypeFromString(type);
    std::string tail = source.substr(lineEnd);

    return { shaderType, tail };
}

u32 OpenGLShader::CompileShader(ShaderType shaderType, const std::string& source) {
    ZPG_PROFILE_FUNCTION();
    const char* sourcePtr = source.c_str();
    u32 shaderID;
    ZPG_OPENGL_CALL( shaderID = glCreateShader(MapShaderTypeToOpenGLenum(shaderType)) );
    ZPG_OPENGL_CALL( glShaderSource(shaderID, 1, &sourcePtr, NULL) );
    ZPG_OPENGL_CALL( glCompileShader(shaderID) );
    
    int status;
    ZPG_OPENGL_CALL(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE) {
        int len;
        ZPG_OPENGL_CALL(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len));
        
        char* msg = new char[len];
        ZPG_OPENGL_CALL(glGetShaderInfoLog(shaderID, len, &len, msg));
        ZPG_OPENGL_CALL(glDeleteShader(shaderID));

        ZPG_CORE_ERROR("Shader compilation failed: {}", m_Name);
        ZPG_CORE_ERROR("{}", msg);
        ZPG_UNREACHABLE();

        delete[] msg;
        return -1;
    }    
    return shaderID;
}

}
