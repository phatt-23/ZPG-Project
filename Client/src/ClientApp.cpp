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
#include "PBRSpheresScene/SpheresScene.h"
#include "TestModelLoadingScene/ModelLoadingScene.h"
#include "TransformHierarchyScene/TransformHierarchyScene.h"
#include "CV5/SpheresScene.h"
#include "CV5/ForestScene.h"

class ClientApp : public ZPG::Application {
public:
    ClientApp() {
        using namespace ZPG;

        ResourceManager::GetGlobal().LoadShaderProgram("basic_lit",
            "./assets/shaders/deprecated/basic_lit.vert",
            "./assets/shaders/deprecated/basic_lit_phong.frag");

        ResourceManager::GetGlobal().LoadShaderProgram("basic_single_color",
            "./assets/shaders/deprecated/basic_lit.vert",
            "./assets/shaders/deprecated/single_color.frag");
            
        ResourceManager::GetGlobal().LoadShaderProgram("basic_normal_color", 
            "./assets/shaders/deprecated/basic_lit.vert",
            "./assets/shaders/deprecated/normal_as_color.frag");
            
        ResourceManager::GetGlobal().LoadShaderProgram("basic_pos_normal_uv", 
            "./assets/shaders/deprecated/basic_pos_normal_uv.vert",
            "./assets/shaders/deprecated/normal_as_color.frag");

        m_SceneManager.AddScene("First", new FirstScene());
        m_SceneManager.AddScene("Second", new SecondScene());
        m_SceneManager.AddScene("TestLights", new LightsScene());
        m_SceneManager.AddScene("TestShaderLoading", new TestShaderLoadingScene());
        m_SceneManager.AddScene("TestMesh", new TestMeshSceneNS::TestMeshScene());
        m_SceneManager.AddScene("TestTexture", new TestTextureSceneNS::TestTextureScene());
        m_SceneManager.AddScene("TestModel", new TestModelSceneNS::TestModelScene());
        m_SceneManager.AddScene("AxisScene", new AxisSceneNS::AxisScene());
        m_SceneManager.AddScene("ForestScene", new ForestSceneNS::ForestScene());
        m_SceneManager.AddScene("Test Model Loading", new TestModelLoadingNS::ModelLoadingScene());
        m_SceneManager.AddScene("Test Skybox", new TransformHierarchyNS::TransformHierarchyScene());
        m_SceneManager.AddScene("Spheres Scene", new PBRSpheresSceneNS::SpheresScene());
        m_SceneManager.AddScene("CV5 - Spheres", new CV5::SpheresScene());
        m_SceneManager.AddScene("CV5 - Forest", new CV5::ForestScene());


        m_SceneManager.SetActiveScene("Spheres Scene");
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

