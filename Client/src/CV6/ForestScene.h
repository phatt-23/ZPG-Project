#pragma once

#include "imgui.h"
#include "ZPGine.h"
#include "../../../assets/models/nemec/plain.h"
#include "../../../assets/models/nemec/tree.h"
#include "../../../assets/models/nemec/bushes.h"
#include "../../../assets/models/nemec/sphere.h"

namespace CV6 {

using namespace ZPG;

class ForestScene final : public ZPG::Scene {
public:
    ForestScene() : m_CameraController(GetCamera()) {
    }

    ResourceManager m_LocalResourceManager;

    void LoadModels() {
        m_LocalResourceManager.LoadModel("Firefly", "./assets/models/sphere/scene.gltf");
        m_LocalResourceManager.LoadModel("Ground", "./assets/models/mud_material/scene.gltf");
        m_LocalResourceManager.LoadModel("Bush", "./assets/models/small_bush/scene.gltf");
        // m_LocalResourceManager.LoadModel("Tree", "./assets/models/pine_tree/scene.gltf");
        m_LocalResourceManager.LoadModel("Tree", "./assets/models/nemec-assets/formula1.obj");
    }

    void OnAttach() override {
        LoadModels();

        m_Flashlight = MakeRef(new SpotLight(v4(1.0), v3(0.0), v3(0.0, 0.0, -1.0), 10.0, 0.5));
        GetLightManager().AddLight(m_Flashlight);

        f32 groundSize = 20.0;

        // trees
        for (int i = 0; i < 50; i++) {
            auto transform = TransformGroup::Build()
                .Add<Rotate>(90, v3(1, 0, 0))
                .Add<Scale>(0.01)
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Add<Rotate>(180.0 * Utility::GetRandomFloat(), v3(0.0, 1.0, 0.0))
                .Compose();

            GetEntityManager().AddEntity(new Entity(m_LocalResourceManager.GetModel("Tree"), transform));
        }

        // bushes
        for (int i = 0; i < 100; i++) {
            auto transform = TransformGroup::Build()
                .Add<Rotate>(-90, v3(1, 0, 0))
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Add<Rotate>(180.0 * Utility::GetRandomFloat(), v3(0.0, 1.0, 0.0))
                .Compose();

            GetEntityManager().AddEntity(new Entity(
                m_LocalResourceManager.GetModel("Bush"),
                transform));
        }

        auto groundTransform = TransformGroup::Build()
            .Add<Rotate>(90, v3(1, 0, 0))
            .Add<Scale>(0.1)
            // .Add<Scale>(groundSize)
            .Compose();

        // ground
        GetEntityManager().AddEntity(new Entity(m_LocalResourceManager.GetModel("Ground"), groundTransform));

        // lights
        m_DirLight = new DirectionalLight(v4(0.1, 0.1, 1.0, 0.8), v3(-1.0));
        AddLight(m_DirLight);

        m_AmbientLight = new AmbientLight(v4(1.0, 1.0, 1.0, 0.001));
        AddLight(m_AmbientLight);

        m_SpotlightEntity = new Entity(
            Model::Create({ GetResourceManager().GetMesh(CommonResources::MESH_SPHERE) }),
            TransformGroup::Build()
                .Add<Scale>(0.1)
                .Add<Translate>(0.0, 1.0, 0.0)
                .Compose()
            );

        AddEntity(m_SpotlightEntity);

        m_SpotLight = MakeRef<SpotLight>(v4(1.0, 0.3, 0.3, 1.0), v3(0.0, 1.0, 0.0), v3(0.0, -1.0, 0.0), 40.0, 0.5);
        AddLight(m_SpotLight);


        AddEntity(new SpotLightEntity(
            m_SpotLight,
            m_LocalResourceManager.GetModel("Firefly"),
            TransformGroup::Build()
                .Add<Scale>(0.05)
                .Add<Translate>(1.0, 0.0, 0.0)
                .Add<DynRotate>(0.0, 20.0, v3(0.0, 1.0, 0.0))
                .Add<Translate>(0.0, 1.0, 0.0)
                .Compose()));

        // fireflies
        for (int i = 0; i < 100; i++) {
            auto pointLight = MakeRef<PointLight>(
                v4(1.0, 1.0, 1.0, 0.2),
                v3(0.0, 1.0, 0.0));

            GetLightManager().AddLight(pointLight);

            auto transform = TransformGroup::Build()
                .Add<Scale>(0.05f)
                .Add<Translate>(
                    Utility::GetRandomFloat() * 5.0f,
                    Utility::GetRandomFloat(0.0, 2.0),
                    Utility::GetRandomFloat() * 5.0f)
                .Add<DynRotate>(0.0f, 20.0f, v3(
                    Utility::GetRandomFloat(0.0, 1.0),
                    5.0,
                    Utility::GetRandomFloat(0.0, 1.0)))
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Compose();

            auto lightEntity = new PointLightEntity(
                pointLight,
                m_LocalResourceManager.GetModel("Firefly"),
                transform);

            GetEntityManager().AddEntity(lightEntity);
        }
        // fireflies
        for (int i = 0; i < 0; i++) {
            auto spotLight = MakeRef<SpotLight>(
                v4(1.0, 1.0, 1.0, 0.2),
                v3(0.0, 1.0, 0.0),
                v3(0.0, -1.0, 0.0),
                40.0,
                0.5);

            GetLightManager().AddLight(spotLight);

            auto transform = TransformGroup::Build()
                .Add<Scale>(0.05f)
                .Add<Translate>(
                    Utility::GetRandomFloat() * 5.0f,
                    Utility::GetRandomFloat(0.0, 1.0),
                    Utility::GetRandomFloat() * 5.0f)
                .Add<DynRotate>(0.0f, 50.0f, v3(
                    Utility::GetRandomFloat(0.0, 1.0),
                    5.0,
                    Utility::GetRandomFloat(0.0, 1.0)))
                .Add<Translate>(
                    groundSize * Utility::GetRandomFloat(),
                    0.0,
                    groundSize * Utility::GetRandomFloat())
                .Compose();

            auto lightEntity = new SpotLightEntity(
                spotLight,
                m_LocalResourceManager.GetModel("Firefly"),
                transform);

            GetEntityManager().AddEntity(lightEntity);
        }
    }

    Entity* m_SpotlightEntity;
    ref<SpotLight> m_SpotLight;
    DirectionalLight* m_DirLight;
    AmbientLight* m_AmbientLight;

    void OnUpdate(Timestep& ts) override {
        m_CameraController.OnUpdate(ts);
        Scene::OnUpdate(ts);

        m_Flashlight->Position.Set(GetCamera().GetPosition());
    }

    void OnEvent(Event &event) override {
        m_CameraController.OnEvent(event);
        Scene::OnEvent(event);
        m_Flashlight->Direction.Set(GetCamera().GetFront());

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(ZPG_FORWARD_EVENT_TO_MEMBER_FN(ForestScene::OnKeyPressed));
    }

    ref<SpotLight> m_Flashlight;
    bool m_FlashlightOn = true;

    bool OnKeyPressed(KeyPressedEvent& event) {
        if (event.GetKeyCode() == ZPG_KEY_F) {
            m_FlashlightOn = !m_FlashlightOn;
            
            ZPG_CORE_DEBUG("Flashlight ON: {}", m_FlashlightOn);

            if (m_FlashlightOn) {
                m_Flashlight->Color.Set(v4(1.0));
            } else {
                m_Flashlight->Color.Set(v4(0.0));
            }
        }
        return false; 
    }

    void OnImGuiRender() override {
        Scene::OnImGuiRender();

        ImGui::Begin("Lights");
        static v4 dirLightColor = m_DirLight->Color.Get();
        if (ImGui::SliderFloat4("DirLight Color", glm::value_ptr(dirLightColor), 0.0, 1.0)) {
            m_DirLight->Color.Set(dirLightColor);
        }
        static v4 ambientLightColor = m_AmbientLight->Color.Get();
        if (ImGui::SliderFloat4("AmbientLight Color", glm::value_ptr(ambientLightColor), 0.0, 1.0)) {
            m_AmbientLight->Color.Set(ambientLightColor);
        }

        static v4 spotLightColor = m_SpotLight->Color.Get();
        if (ImGui::SliderFloat4("SpotLight Color", glm::value_ptr(spotLightColor), 0.0, 1.0)) {
            m_SpotLight->Color.Set(spotLightColor);
        }

        static v3 spotlightDir = m_SpotLight->Direction.Get();
        if (ImGui::SliderFloat3("SpotLight Direction", glm::value_ptr(spotlightDir), -1.0, 1.0)) {
            m_SpotLight->Direction.Set(spotlightDir);
        }

        static v3 spotlightPos = m_SpotLight->Position.Get();
        if (ImGui::SliderFloat3("SpotLight Position", glm::value_ptr(spotlightPos), -1.0, 1.0)) {
            m_SpotLight->Position.Set(spotlightPos);
        }

        static float spotlightBeamSize = m_SpotLight->BeamShape.GetSize();
        if (ImGui::SliderFloat("SpotLight Beam Size", &spotlightBeamSize, 0.0, 180.0)) {
            m_SpotLight->BeamShape.SetSize(spotlightBeamSize);
        }

        static float spotlightBeamBlend = m_SpotLight->BeamShape.GetBlend();
        if (ImGui::SliderFloat("SpotLight Beam Blend", &spotlightBeamBlend, 0.0, 1.0)) {
            m_SpotLight->BeamShape.SetBlend(spotlightBeamBlend);
        }

        v3 f = GetCamera().GetFront();
        ImGui::Text("Camera Front: %f, %f, %f", f.x, f.y, f.z);

        ImGui::End();
    }

    ref<ShaderProgram> m_ShaderProgram;
    CameraController m_CameraController;
};

}
