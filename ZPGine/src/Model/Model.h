#pragma once

#include "Mesh.h"

namespace ZPG {


class Model {
public:
    Model(const std::vector<Ref<Mesh>>& meshes) 
    : m_Meshes(meshes) {

    }
    ~Model() {}

    const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
private:
    std::vector<Ref<Mesh>> m_Meshes;
};

}