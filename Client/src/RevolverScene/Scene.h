#include "ZPGine.h"

namespace RevolverScene {

using namespace ZPG;
    
class RevolverScene : public Scene {
public:
    RevolverScene();

    void OnLazyAttach() override;

    void OnImGuiRender() override;

private:
    ResourceManager m_LocalRes;
    char* m_CurrentShaderProgramRoute;
    v4 firefly_color = v4(1.0);
    ref<Material> firefly_material;
    std::vector<ref<PointLight>> firefly_lights;
    AmbientLight* ambient_light = nullptr;
    DirectionalLight* dir_light = nullptr;
};

}
