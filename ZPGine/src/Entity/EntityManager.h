#pragma once

#include "Entity.h"

namespace ZPG {

class EntityManager {
public:
    EntityManager() {}
    ~EntityManager() {}
    void AddEntity(Entity* entity) { 
        m_Entities.push_back(CreateScope(entity)); 
    }

    const std::vector<Scope<Entity>>& GetEntities() const { 
        return m_Entities; 
    }
    
private:
    std::vector<Scope<Entity>> m_Entities;
};

}