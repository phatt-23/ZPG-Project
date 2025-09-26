//
// Created by phatt on 9/22/25.
//

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glm/glm.hpp>
#include "Core/Core.h"

namespace ZPG {

class ShaderProgram {
public:
    virtual ~ShaderProgram() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual void SetInt(const std::string& uniName, glm::i32 value) = 0;
    virtual void SetInt2(const std::string& uniName, glm::i32vec2 vec) = 0;
    virtual void SetInt3(const std::string& uniName, glm::i32vec3 vec) = 0;
    virtual void SetInt4(const std::string& uniName, glm::i32vec4 vec) = 0;
    virtual void SetFloat(const std::string& uniName, glm::f32 value) = 0;
    virtual void SetFloat2(const std::string& uniName, glm::f32vec2 vec) = 0;
    virtual void SetFloat3(const std::string& uniName, glm::f32vec3 vec) = 0;
    virtual void SetFloat4(const std::string& uniName, glm::f32vec4 vec) = 0;
    virtual void SetMat3(const std::string& uniName, glm::mat3 mat) = 0;
    virtual void SetMat4(const std::string& uniName, glm::mat4 mat) = 0;

    static Ref<ShaderProgram> Create(const std::string& path);
    static Ref<ShaderProgram> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
private:
};

}
#endif //SHADER_PROGRAM_H
