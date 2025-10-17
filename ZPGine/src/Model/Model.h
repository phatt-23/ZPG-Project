#pragma once

#include "Mesh.h"

namespace ZPG {


class Model {
public:
    Model(const std::vector<ref<Mesh>>& meshes) 
    : m_Meshes(meshes) {
    }

    ~Model() {}

    const std::vector<ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    static ref<Model> Create(const std::vector<ref<Mesh>>& meshes);
private:
    std::vector<ref<Mesh>> m_Meshes;
};

}
