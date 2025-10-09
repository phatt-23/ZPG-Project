//
// Created by phatt on 9/22/25.
//

#include "ZPGine.h"

#include <imgui.h>
#include "FirstScene/FirstScene.h"
#include "SecondScene/SecondScene.h"
#include "TestShaderLoadingScene/TestShaderLoadingScene.h"
#include "TestLightsScene/LightsScene.h"
#include "TestTextureScene/TestTextureScene.h"
#include "TestMeshScene/TestMeshScene.h"
#include "TestModelScene/TestModelScene.h"
#include "AxisScene/AxisScene.h"
#include "ForestScene/ForestScene.h"
#include "TestModelLoadingScene/ModelLoadingScene.h"

class ClientApp : public ZPG::Application {
public:
    ClientApp() {
        using namespace ZPG;

        // ------ add shader programs -------
        ResourceManager::GetGlobal().LoadShaderProgram("basic_lit", 
            "./assets/shaders/vertex/basic_lit.vert", 
            "./assets/shaders/fragment/basic_lit_phong.frag");

        ResourceManager::GetGlobal().LoadShaderProgram("basic_single_color",
            "./assets/shaders/vertex/basic_lit.vert", 
            "./assets/shaders/fragment/single_color.frag");
            
        ResourceManager::GetGlobal().LoadShaderProgram("basic_normal_color", 
            "./assets/shaders/vertex/basic_lit.vert", 
            "./assets/shaders/fragment/normal_as_color.frag");
            
        ResourceManager::GetGlobal().LoadShaderProgram("basic_pos_normal_uv", 
            "./assets/shaders/vertex/basic_pos_normal_uv.vert", 
            "./assets/shaders/fragment/normal_as_color.frag");

        // ------ add scenes -------
        m_SceneManager.AddScene("1. First", new FirstScene());
        m_SceneManager.AddScene("2. Second", new SecondScene());
        m_SceneManager.AddScene("3. TestLights", new LightsScene());
        m_SceneManager.AddScene("4. TestShaderLoading", new TestShaderLoadingScene());
        m_SceneManager.AddScene("5. TestMesh", new TestMeshSceneNS::TestMeshScene());
        m_SceneManager.AddScene("6. TestTexture", new TestTextureSceneNS::TestTextureScene());
        m_SceneManager.AddScene("7. TestModel", new TestModelSceneNS::TestModelScene());
        m_SceneManager.AddScene("8. AxisScene", new AxisSceneNS::AxisScene());
        m_SceneManager.AddScene("9. ForestScene", new ForestSceneNS::ForestScene());
        m_SceneManager.AddScene("10. Test Model Loading", 
            new TestModelLoadingNS::ModelLoadingScene());

    }

    void OnImGuiRender() override {
        ImGui::Begin("Scene Switcher");
            for (auto& [sceneName, sceneInstance] : m_SceneManager) {
                if (ImGui::Button(sceneName.c_str())) {
                    m_SceneManager.SetActiveScene(sceneName);
                }
            }
        ImGui::End();
    }
};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // main function gets put here

