#include "Model.h"
#include "Transform/CompoundTransform.h"

namespace ZPG {

Model::Model() {
}
Model::~Model() {
}
void Model::ForEachNode(const VisitorFn& visitor) {
    ZPG_NOT_IMPL();
    // // construct a unit transformation
    // Ref<Transform> transform = CreateRef<Transform>(new CompoundTransform());
    //
    // // begin traversing from the root node
    // Traverse(m_RootNode, transform, visitor);
}
void Model::Traverse(Ref<ModelNode>& node, Ref<Transform>& parentTransform, const VisitorFn& visitor) {
    ZPG_NOT_IMPL();
    // // compose the transformatoins
    // CompoundTransform* worldComp = new CompoundTransform();
    // worldComp->Push(node->GetTransform());
    // worldComp->Push(parentTransform);
    //
    // Transform* worldPtr = worldComp;
    // Ref<Transform> worldTransform = CreateRef<Transform>(worldPtr);
    //
    // // visit the current node
    // visitor(node, worldTransform);
    //
    // // visit its children
    // for (auto child : node->GetChildren()) {
    //     Traverse(child, worldTransform, visitor);
    // }
}

}
