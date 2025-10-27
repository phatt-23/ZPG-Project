#pragma once

namespace ZPG {

class ShaderProgram;

// Could store scoped shader programs because the ShaderProgramLibrary dictates the 
// lifetime of its ShaderPrograms. And return raw pointers for less overhead (others should of course not call delete on it).
// But I'm already using Ref<T> and Scope<T> memory system in my project and don't work with raw pointers.
// So my choice is to just use Ref<T>.
class ShaderProgramLibrary {
public:
    ShaderProgramLibrary();
    ~ShaderProgramLibrary();

    void AddShaderProgram(const std::string& name, const ref<ShaderProgram>& ShaderProgram);

    // non const because other subjects can set uniforms
    ref<ShaderProgram>& GetShaderProgram(const std::string& name);
    bool Exists(const std::string& name) const;
    const std::unordered_map<std::string, std::shared_ptr<ShaderProgram>>& GetShaders() const;

private:
    std::unordered_map<std::string, ref<ShaderProgram>> m_ShaderProgramMap;
};

}
