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

class ClientApp : public ZPG::Application {
public:
    ClientApp() {
        m_SceneManager.AddScene("Second", ZPG::CreateRef<SecondScene>());
        m_SceneManager.AddScene("First", ZPG::CreateRef<FirstScene>());
        m_SceneManager.AddScene("TestLights", ZPG::CreateRef<LightsScene>());
        m_SceneManager.AddScene("TestShaderLoading", ZPG::CreateRef<TestShaderLoadingScene>());
        m_SceneManager.AddScene("TestMesh", ZPG::CreateRef<TestMeshSceneNS::TestMeshScene>());
        m_SceneManager.AddScene("TestTexture", ZPG::CreateRef<TestTextureSceneNS::TestTextureScene>());
        m_SceneManager.AddScene("TestModel", ZPG::CreateRef<TestModelSceneNS::TestModelScene>());
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
private:
};

ZPG::Application* ZPG::CreateApplication() {
    return new ClientApp();
}

#include "Core/EntryPoint.h"    // main function gets put here

