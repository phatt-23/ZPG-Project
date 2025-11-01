#include "ZPGine.h"

namespace HyenaScene {

using namespace ZPG;
    
class HyenaScene : public Scene {
public:
    HyenaScene();

    void OnAttach() override;

    void OnImGuiRender() override;

private:
    ResourceManager m_LocalRes;
    char* m_CurrentShaderProgramRoute;
    v4 fireflyColor = v4(1.0);
    ref<Material> fireflyMaterial;
    std::vector<ref<PointLight>> fireflyLights;
};

}
