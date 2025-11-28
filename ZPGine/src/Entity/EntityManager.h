#pragma once

namespace ZPG
{

    class Entity;

    class EntityManager
    {
    public:
        EntityManager();
        ~EntityManager();

        void AddEntity(Entity* entity);
        void AddEntity(const ref<Entity>& entity);
        void AddStaticEntity(const ref<Entity>& entity);
        void AddStaticEntity(Entity* entity);
        const std::vector<ref<Entity>>& GetEntities() const;
        const std::vector<ref<Entity>>& GetStaticEntities() const { return m_StaticEntities; }

        ref<Entity> GetEntity(int entityID) const;
        void RemoveEntity(int entityID);

        void Clear();

    private:
        std::vector<ref<Entity>> m_Entities;
        std::vector<ref<Entity>> m_StaticEntities;
    };

}
