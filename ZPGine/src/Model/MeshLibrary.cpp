#include "MeshLibrary.h"

#include "Debug/Asserter.h"
#include "Mesh.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

void MeshLibrary::AddMesh(const std::string& name, const ref<Mesh>& mesh) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(!Exists(name), "Mesh library already contains a mesh with the name: %s", name);
    m_Meshes[name] = mesh;
}

ref<Mesh> const& MeshLibrary::GetMesh(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(Exists(name), "Mesh library doesn't contain a mesh with the name: %s", name.c_str());
    return m_Meshes.at(name);
}

const std::unordered_map<std::string, ref<Mesh>>& MeshLibrary::GetMeshes() const {
    ZPG_PROFILE_FUNCTION();
    return m_Meshes;
}

bool MeshLibrary::Exists(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_Meshes.contains(name);
}

}
