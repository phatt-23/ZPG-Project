#include "ZPGine.h"

namespace HappyGhastScene {

using namespace ZPG;
    
class HappyGhastScene : public Scene {
public:
    HappyGhastScene();

    void OnLazyAttach() override;

    void OnUpdate(Timestep &ts) override;

    void OnEvent(Event &event) override;

    void OnImGuiRender() override;

private:
    CameraController m_Controller;
    ResourceManager m_LocalRes;
    char* m_CurrentShaderProgramRoute;
    v4 firefly_color = v4(1.0);
    ref<Material> firefly_material;
    std::vector<ref<PointLight>> firefly_lights;
    AmbientLight* ambient_light = nullptr;
    DirectionalLight* dir_light = nullptr;
};

}
