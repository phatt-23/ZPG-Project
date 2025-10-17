#pragma once

#include "Entity.h"

namespace ZPG {

class EntityManager {
public:
    EntityManager() {}
    ~EntityManager() {}

    void AddEntity(Entity* entity) { 
        m_Entities.push_back(MakeRef(entity));
    }

    void AddEntity(const ref<Entity>& entity) {
        m_Entities.push_back(entity);
    }

    const std::vector<ref<Entity>>& GetEntities() const {
        return m_Entities; 
    }
    
private:
    std::vector<ref<Entity>> m_Entities;
};

}