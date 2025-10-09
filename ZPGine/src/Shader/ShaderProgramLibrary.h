#pragma once
#include "ShaderProgram.h"
#include <unordered_map>

namespace ZPG {

// Could store scoped shader programs because the ShaderProgramLibrary dictates the 
// lifetime of its ShaderPrograms. And return raw pointers for less overhead (others should of course not call delete on it).
// But Im already using Ref<T> and Scope<T> memory system in my project and don't work with raw pointers.
// So my choice is to just use Ref<T>.
class ShaderProgramLibrary {
public:
    ShaderProgramLibrary();
    ~ShaderProgramLibrary();

    void AddShaderProgram(const std::string& name, const Ref<ShaderProgram>& ShaderProgram);

    // non const because other subjects can set uniforms
    Ref<ShaderProgram>& GetShaderProgram(const std::string& name);
    bool Exists(const std::string& name) const;
private:
    std::unordered_map<std::string, Ref<ShaderProgram>> m_ShaderProgramMap;
};

}
