//
// Created by phatt on 11/8/25.
//

#ifndef WORKSPACE_SHADOWSCENE_H
#define WORKSPACE_SHADOWSCENE_H
#include "Scene/Scene.h"


namespace CV8
{
    class ShadowScene : public ZPG::Scene
    {
    public:

        ShadowScene();
        void OnAttach() override;
        void OnEvent(ZPG::Event& event) override;

    private:

        ZPG::ResourceManager m_LocalResources;
    };
}


#endif //WORKSPACE_SHADOWSCENE_H