#include "Model.h"

namespace ZPG {


Model::Model(const std::vector<ref<Mesh>>& meshes)
: m_Meshes(meshes) {
}

Model::~Model() {}

const std::vector<ref<Mesh>>& Model::GetMeshes() const {
    return m_Meshes;
}

ref<Model> Model::Create(const std::vector<ref<Mesh>>& meshes) {
    Model* model = new Model(meshes);
    return MakeRef(model);
}

}
