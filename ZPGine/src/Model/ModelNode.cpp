#include "ModelNode.h"

namespace ZPG {

ModelNode::ModelNode(const Ref<Mesh>& mesh, const Ref<Transform>& transform) 
: m_Mesh(mesh), m_Transform(transform), m_Children() {
}
ModelNode::ModelNode(const Ref<Mesh>& mesh, const Ref<Transform>& transform, const std::vector<Ref<ModelNode>>& children)
: m_Mesh(mesh), m_Transform(transform), m_Children(children) {
}
ModelNode::~ModelNode() {
}

}
