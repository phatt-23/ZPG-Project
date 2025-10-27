//
// Created by phatt on 10/24/25.
//

#ifndef WORKSPACE_POINT_LIGHT_ENTITY_H
#define WORKSPACE_POINT_LIGHT_ENTITY_H
#include "Entity.h"

namespace ZPG {

class PointLight;

class PointLightEntity : public Entity {
public:
    PointLightEntity(const ref<PointLight>& pointLight, const ref<Model>& model, const ref<Transform>& transform);
    ~PointLightEntity() override;

    void Update(Timestep& ts) override;
private:
    ref<PointLight> m_PointLight;
};

}


#endif