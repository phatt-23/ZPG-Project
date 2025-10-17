#pragma once

namespace ZPG {

class Entity;

class EntityManager {
public:
    EntityManager();
    ~EntityManager();

    void AddEntity(Entity* entity);
    void AddEntity(const ref<Entity>& entity);
    const std::vector<ref<Entity>>& GetEntities() const;

private:
    std::vector<ref<Entity>> m_Entities;
};

}