#pragma once

#include "Model/ModelNode.h"
#include <functional>

namespace ZPG {

class Model {
public:
    using VisitorFn = std::function<void(Ref<ModelNode>& node, Ref<Transform>& world)>;

    Model();
    ~Model();

    const Ref<ModelNode>& GetRootNode() const { return m_RootNode; }
    Ref<ModelNode>& GetRootNode() { return m_RootNode; }
    
    void ForEachNode(const VisitorFn& visitor);
private:
    void Traverse(Ref<ModelNode>& node, Ref<Transform>& parentTransform, const VisitorFn& visitor);
private:
    Ref<ModelNode> m_RootNode = nullptr;
};

}
