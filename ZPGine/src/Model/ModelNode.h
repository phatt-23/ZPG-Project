#pragma once

#include "Model/Mesh.h"
#include "Transform/Transform.h"
namespace ZPG {

class ModelNode {
public:
    ModelNode();
    ModelNode(const Ref<Mesh>& mesh, const Ref<Transform>& transform);
    ModelNode(const Ref<Mesh>& mesh, const Ref<Transform>& transform, const std::vector<Ref<ModelNode>>& children);
    ~ModelNode();

    const Ref<Mesh>& GetMesh() const { return m_Mesh; }
    const Ref<Transform>& GetTransform() const { return m_Transform; }
    const std::vector<Ref<ModelNode>>& GetChildren() const { return m_Children; }
    void AddChild(Ref<ModelNode> const& child) { m_Children.push_back(child); }
private:
    Ref<Mesh> m_Mesh;
    Ref<Transform> m_Transform;
    std::vector<Ref<ModelNode>> m_Children;
};

}
