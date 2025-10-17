#include "Model.h"

namespace ZPG {

ref<Model> Model::Create(const std::vector<ref<Mesh>>& meshes) {
    Model* model = new Model(meshes);
    return MakeRef<>(model);
}

}
