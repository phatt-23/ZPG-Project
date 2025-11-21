#pragma once
#include "ZPGine.h"

namespace CV8
{

    class SkydomeScene : public ZPG::Scene {
    public:
        SkydomeScene();
        void OnAttach() override;

    private:
        ZPG::ResourceManager m_LocalResources;
    };

}
