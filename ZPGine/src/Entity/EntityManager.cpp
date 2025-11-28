#include "EntityManager.h"
#include "Entity.h"
#include "Profiling/Instrumentor.h"

namespace ZPG
{

    EntityManager::EntityManager()
    {
    }

    EntityManager::~EntityManager()
    {
    }

    void EntityManager::AddEntity(Entity* entity)
    {
        m_Entities.push_back(MakeRef(entity));
    }

    void EntityManager::AddEntity(const ref<Entity>& entity)
    {
        m_Entities.push_back(entity);
    }

    void EntityManager::AddStaticEntity(const ref<Entity>& entity)
    {
        m_StaticEntities.push_back(entity);
    }

    void EntityManager::AddStaticEntity(Entity* entity)
    {
        m_StaticEntities.push_back(MakeRef(entity));
    }

    const std::vector<ref<Entity>>& EntityManager::GetEntities() const
    {
        return m_Entities;
    }

    ref<Entity> EntityManager::GetEntity(int entityID) const
    {
        auto it = std::ranges::find_if(m_Entities, [entityID](const ref<Entity>& e){
            return e->GetEntityID() == entityID;
        });
    
        if (it != m_Entities.end())
            return *it;

        it = std::ranges::find_if(m_StaticEntities, [entityID](const ref<Entity>& e){
            return e->GetEntityID() == entityID;
        });
        
        if (it != m_StaticEntities.end())
            return *it;

        return nullptr;
    }

    void EntityManager::RemoveEntity(int entityID)
    {
        auto it = std::ranges::find_if(m_Entities, [entityID](const ref<Entity>& entity)
        {
            return entity->GetEntityID() == entityID;
        });

        if (it != m_Entities.end())
        {
            m_Entities.erase(it);
        }
    }

    void EntityManager::Clear()
    {
        m_Entities.clear();
    }

}
