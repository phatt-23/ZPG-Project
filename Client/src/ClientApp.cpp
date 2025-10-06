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

class ClientApp : public ZPG::Application {
public:
    ClientApp() {
        // ------ add scenes -------
        m_SceneManager.AddScene("1. First", ZPG::CreateRef<FirstScene>());
        m_SceneManager.AddScene("2. Second", ZPG::CreateRef<SecondScene>());
        m_SceneManager.AddScene("3. TestLights", ZPG::CreateRef<LightsScene>());
        m_SceneManager.AddScene("4. TestShaderLoading", ZPG::CreateRef<TestShaderLoadingScene>());
        m_SceneManager.AddScene("5. TestMesh", ZPG::CreateRef<TestMeshSceneNS::TestMeshScene>());
        m_SceneManager.AddScene("6. TestTexture", ZPG::CreateRef<TestTextureSceneNS::TestTextureScene>());
        m_SceneManager.AddScene("7. TestModel", ZPG::CreateRef<TestModelSceneNS::TestModelScene>());
        m_SceneManager.AddScene("8. AxisScene", ZPG::CreateRef<AxisSceneNS::AxisScene>());

        // ------ add shader programs -------
        Renderer::LoadShaderProgram("basic_lit", 
                                        "./assets/shaders/vertex/basic_lit.vert", 
                                        "./assets/shaders/fragment/basic_lit_phong.frag");
        Renderer::LoadShaderProgram("basic_single_color", 
                                        "./assets/shaders/vertex/basic_lit.vert", 
                                        "./assets/shaders/fragment/single_color.frag");
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

