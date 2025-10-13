#include "Model.h"

namespace ZPG {

Ref<Model> Model::Create(const std::vector<Ref<Mesh>>& meshes) {
    Model* model = new Model(meshes);
    return CreateRef<>(model);
}

}
