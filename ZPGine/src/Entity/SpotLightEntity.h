//
// Created by phatt on 10/26/25.
//

#ifndef WORKSPACE_SPOTLIGHT_ENTITY_H
#define WORKSPACE_SPOTLIGHT_ENTITY_H
#include "Entity.h"

namespace ZPG {
class SpotLight;

class SpotLightEntity : public Entity {
public:
    SpotLightEntity(const ref<SpotLight>& spotLight, const ref<Model>& model, const ref<Transform>& transform);
    ~SpotLightEntity() override;

    void Update(Timestep& ts) override;
private:
    ref<SpotLight> m_SpotLight;
};

}

#endif