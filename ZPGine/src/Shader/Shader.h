#pragma once

namespace ZPG {

class Shader {
public:
    enum ShaderType { 
        None = ZPG_BIT(0), 
        Vertex = ZPG_BIT(1), 
        Fragment = ZPG_BIT(2), 
        Geometry = ZPG_BIT(3), 
    };
    static std::string ShaderTypeToString(ShaderType type);
    static ShaderType ShaderTypeFromString(const std::string& type);

    virtual ~Shader() = default;
    virtual ShaderType GetShaderType() const = 0;

    virtual void AttachTo(u32 shaderProgramID) const = 0;
    virtual void DetachFrom(u32 shaderProgramID) const = 0;

    static ref<Shader> Create(const std::string& filepath);
    static ref<Shader> Create(const std::string& name, const std::string& filepath);
    static ref<Shader> CreateFromCode(const std::string& name, ShaderType type, const std::string& sourceCode);
private:

};

}
