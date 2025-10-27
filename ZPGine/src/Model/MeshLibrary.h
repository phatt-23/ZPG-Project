#pragma once
#include <string>

#include "Mesh.h"
#include "Core/Core.h"

namespace ZPG {

class Mesh;

class MeshLibrary {
public:
    void AddMesh(const std::string& name, const ref<Mesh>& mesh);
    ref<Mesh> const& GetMesh(const std::string& name) const;
    const std::unordered_map<std::string, ref<Mesh>>& GetMeshes() const;
    bool Exists(const std::string& name) const;

private:
    std::unordered_map<std::string, ref<Mesh>> m_Meshes;
};

}
