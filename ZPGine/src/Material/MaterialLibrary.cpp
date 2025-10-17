#include "MaterialLibrary.h"
#include "Material.h"
#include "Debug/Asserter.h"

namespace ZPG {

void MaterialLibrary::AddMaterial(const std::string& name, const ref<Material>& material) {
    ZPG_CORE_ASSERT(!Exists(name), "Material with this is already in the library: {}", name);
    m_Materials[name] = material;
}
bool MaterialLibrary::Exists(const std::string& name) const {
    return m_Materials.contains(name);
}
const ref<Material>& MaterialLibrary::GetMaterial(const std::string& name) {
    ZPG_CORE_ASSERT(Exists(name), "Material with this name isn't in the library: {}", name);
    return m_Materials[name];
}

}
