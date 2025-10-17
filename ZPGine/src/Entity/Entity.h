#pragma once

namespace ZPG {

class Model;
class Transform;
class Timestep;

class Entity {
public:
    Entity(const ref<Model>& model, const ref<Transform>& transform);
    virtual ~Entity();
    void Update(Timestep& ts);
    const glm::mat4& GetTransformMatrix() const;
    const ref<Transform>& GetTransform() const;
    const ref<Model>& GetModel() const;

private:
    ref<Model> m_Model;
    ref<Transform> m_Transform;
};

}
