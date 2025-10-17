#include "EntityManager.h"
#include "Entity.h"

namespace ZPG {

EntityManager::EntityManager() {}
EntityManager::~EntityManager() {}

void EntityManager::AddEntity(Entity* entity) {
    m_Entities.push_back(MakeRef(entity));
}

void EntityManager::AddEntity(const ref<Entity>& entity) {
    m_Entities.push_back(entity);
}

const std::vector<ref<Entity>>& EntityManager::GetEntities() const {
    return m_Entities;
}

}
