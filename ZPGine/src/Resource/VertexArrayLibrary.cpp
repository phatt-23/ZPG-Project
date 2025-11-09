//
// Created by phatt on 10/26/25.
//

#include "VertexArrayLibrary.h"

#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

void VertexArrayLibrary::AddVAO(const std::string& name, const ref<VertexArray>& vao) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(!Exists(name), "VAO library already contains a VAO with the name: %s", name);
    m_VAOs[name] = vao;
}

const ref<VertexArray>& VertexArrayLibrary::GetVAO(const std::string& name)  const {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(Exists(name), "VAO library doesn't contain a VAO with the name: %s", name);
    return m_VAOs.at(name);
}

const std::unordered_map<std::string, ref<VertexArray>>& VertexArrayLibrary::GetVAOs() const {
    ZPG_PROFILE_FUNCTION();
    return m_VAOs;
}

bool VertexArrayLibrary::Exists(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_VAOs.contains(name);
}

}
