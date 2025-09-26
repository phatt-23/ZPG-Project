#include "OpenGLShaderProgram.h"
#include "OpenGLCore.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Debug/Asserter.h"

namespace ZPG {

static std::string GetNameFromPath(const std::string& path) {
    size_t lastSlash = path.find_last_of("/\\");  // linux and windows slash
    size_t lastDot = path.rfind(".");
    auto nameBegin = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto nameEnd = lastDot == std::string::npos ? path.size() : lastDot;
    return path.substr(nameBegin, nameEnd - nameBegin);
}

static std::string ReadFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary); 

    f.seekg(0, std::ios::end);
    std::string content;
    content.resize(f.tellg());
    f.seekg(0, std::ios::beg);
    f.read(content.data(), content.size());
    f.close();

    return std::move(content);
}

std::unordered_map<u32, std::string> OpenGLShaderProgram::PreProcess(const std::string& input) {
    using namespace std;
    unordered_map<u32, std::string> shaders;

    const char* typeTok = "#type";

    size_t p = 0;
    p = input.find(typeTok, p);

    while (p != string::npos) {
        size_t lineEnd = min(input.find("\n", p), input.find("\r\n"));
        size_t afterTok = p + strlen(typeTok);

        ZPG_CORE_ASSERT(afterTok < lineEnd, "After #type is nothing.")
        ZPG_CORE_ASSERT(input[afterTok] == ' ', "Missing space after #type.");

        size_t b = input.find_first_not_of(' ', afterTok);
        size_t e = min({ input.find(" ", b), input.find("//", b), lineEnd });

        const string& t = input.substr(b, e - b);

        GLenum shaderType = -1;
        if (t == "vertex") shaderType = GL_VERTEX_SHADER;
        else if (t == "geometry") shaderType = GL_GEOMETRY_SHADER;
        else if (t == "fragment" || t == "pixel") shaderType = GL_FRAGMENT_SHADER;

        ZPG_CORE_ASSERT(shaderType != -1, "Shader type found after #type not supported: {}", t);

        size_t nextLine = input.find_first_not_of("\r\n", lineEnd);
        size_t nextTok = input.find(typeTok, nextLine);

        const string& shaderSource = input.substr(nextLine, (nextTok == string::npos ? input.size() : nextTok) - nextLine);
        shaders[shaderType] = shaderSource;

        p = input.find(typeTok, nextLine);
    }

    ZPG_CORE_ASSERT(!shaders.empty(), "There is no #type specification in the source.");
    return shaders;
}

u32 OpenGLShaderProgram::Compile(std::unordered_map<u32, std::string> sources) {
    u32 program;
    ZPG_OPENGL_CALL(program = glCreateProgram());

    std::vector<u32> shaders(sources.size());
    for (auto& [type, source] : sources) {
        u32 shaderId = CompileShader(type, source);
        shaders.push_back(shaderId);
        ZPG_OPENGL_CALL(glAttachShader(program, shaderId));
    }

    ZPG_OPENGL_CALL(glLinkProgram(program));

    i32 status;
    ZPG_OPENGL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &status));
    if (status == GL_FALSE) {
        i32 len;
        ZPG_OPENGL_CALL(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len)); 
        
        char* msg = new char[len];
        ZPG_OPENGL_CALL(glGetProgramInfoLog(program, len, &len, msg));

        ZPG_OPENGL_CALL(glDeleteProgram(program));
        for (u32 shader : shaders) {
            ZPG_OPENGL_CALL(glDeleteShader(shader));
        }

        ZPG_UNREACHABLE("ShaderProgram linking failed: {}", msg);

        delete[] msg;
        return 0;
    }
    
    for (u32 shId : shaders) {
        ZPG_OPENGL_CALL(glDeleteShader(shId));
    }

    return program;
}

u32 OpenGLShaderProgram::CompileShader(u32 type, const std::string& source) {
    u32 shId;
    const char* sourcePtr = source.c_str();
    ZPG_OPENGL_CALL(shId = glCreateShader(type));
    ZPG_OPENGL_CALL(glShaderSource(shId, 1, &sourcePtr, NULL));
    ZPG_OPENGL_CALL(glCompileShader(shId));

    int status;
    ZPG_OPENGL_CALL(glGetShaderiv(shId, GL_COMPILE_STATUS, &status));
    if (status == GL_FALSE) {
        int len;
        ZPG_OPENGL_CALL(glGetShaderiv(shId, GL_INFO_LOG_LENGTH, &len));
        
        char* msg = new char[len];
        ZPG_OPENGL_CALL(glGetShaderInfoLog(shId, len, &len, msg));
        ZPG_OPENGL_CALL(glDeleteShader(shId));

        ZPG_CORE_ERROR("Shader compilation failed.");
        ZPG_CORE_ERROR("{}", msg);
        ZPG_UNREACHABLE();

        delete[] msg;
        return 0;
    }

    return shId;
}

OpenGLShaderProgram::OpenGLShaderProgram(const std::string& path) {
    m_Name = GetNameFromPath(path);
    m_RendererID = Compile(PreProcess(ReadFile(path)));
}
OpenGLShaderProgram::OpenGLShaderProgram(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) {
    m_Name = name;
    m_RendererID = Compile({
        {GL_VERTEX_SHADER, vertexSource}, 
        {GL_FRAGMENT_SHADER, fragmentSource}
    });
}
OpenGLShaderProgram::~OpenGLShaderProgram() {
    ZPG_OPENGL_CALL( glDeleteProgram(m_RendererID) );
}

void OpenGLShaderProgram::Bind() {
    ZPG_OPENGL_CALL( glUseProgram(m_RendererID) );
}
void OpenGLShaderProgram::Unbind() {
    ZPG_OPENGL_CALL( glUseProgram(0) );
}

i32 OpenGLShaderProgram::GetUniformLocation(const std::string &name) {
    if (m_LocationCache.contains(name))
        return m_LocationCache[name];

    ZPG_OPENGL_CALL(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        ZPG_CORE_ERROR("Shader ({}): Uniform with name '{}' wasn't found!", m_RendererID, name);

    m_LocationCache[name] = location;
    return location;
}

void OpenGLShaderProgram::SetInt(const std::string& uniName, glm::i32 scalar) {
    ZPG_OPENGL_CALL(glUniform1iv(GetUniformLocation(uniName), 1, &scalar));
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
void OpenGLShaderProgram::SetFloat(const std::string& uniName, glm::f32 scalar) {
    ZPG_OPENGL_CALL(glUniform1fv(GetUniformLocation(uniName), 1, &scalar));
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

}
