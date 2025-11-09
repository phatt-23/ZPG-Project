#include "Model.h"

#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {


Model::Model(const std::vector<ref<Mesh>>& meshes)
: m_Meshes(meshes) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(meshes.size() > 0, "Model must contain at least one mesh.");
}

Model::~Model() {}


const std::string& Model::GetName() const {
    ZPG_PROFILE_FUNCTION();
    return m_Name;
}

void Model::SetName(std::string const& name) {
    ZPG_PROFILE_FUNCTION();
    m_Name = name;
}

const std::vector<ref<Mesh>>& Model::GetMeshes() const {
    ZPG_PROFILE_FUNCTION();
    return m_Meshes;
}

ref<Model> Model::Create(const std::vector<ref<Mesh>>& meshes) {
    ZPG_PROFILE_FUNCTION();
    Model* model = new Model(meshes);
    return MakeRef(model);
}


ref<Model> Model::Create(const std::string& name, const std::vector<ref<Mesh>>& meshes) {
    ZPG_PROFILE_FUNCTION();
    Model* model = new Model(meshes);
    model->SetName(name);
    return MakeRef(model);
}

}
