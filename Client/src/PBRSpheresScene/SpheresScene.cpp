//
// Created by phatt on 10/16/25.
//

#include "SpheresScene.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "SpheresLayer.h"
#include "../../../assets/models/nemec/suzi_smooth.h"
#include "../assets/models/nemec/sphere.h"

using namespace ZPG;

namespace PBRSpheresSceneNS {

SpheresScene::SpheresScene() : m_CameraController(GetCamera()) {
}

void SpheresScene::OnAttach() {
    PushLayer(new SpheresLayer());

    AddLight(new AmbientLight(v4(1.0, 1.0, 1.0, 0.3)));
    AddLight(new DirectionalLight(v4(1.0, 1.0, 1.0, 1.0), v3(-1.0, -1.0, 1.0)));
    AddLight(new PointLight(v4(1.0f), v3(0.0, 0.0, 3.0)));


    auto PBRshader = GetResourceManager().GetShaderProgram(CommonResources::SHADER_PROGRAM_PBR);

    int gridSize = 5;
    float dist = 1.5;

    auto vao = VertexArray::Create({
        VertexBuffer::Create(nemec::sphere, sizeof(nemec::sphere), {
            {ShaderDataType::Float3, "a_Pos"},
            {ShaderDataType::Float3, "a_Normal"},
        })
    });

    // Entity* parentEnt = nullptr;

    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            auto mesh = Mesh::Create(vao);

            auto material = new Material();

            f32 fi = (f32)i / (f32)(gridSize - 1);
            f32 fj = (f32)j / (f32)(gridSize - 1);

            material->SetShaderProgram(PBRshader);
            material->SetAlbedo(v4(1.0, 0.5, 0.5, 1.0));
            material->SetMetallic(fi);
            material->SetRoughness(fj);

            mesh->SetMaterial(MakeRef(material));

            ref<Model> sphereModel = Model::Create({ mesh });

            auto transform = TransformGroup::Build()
                // .WithParent(parentEnt ? parentEnt->GetTransform() : nullptr)
                .Add<Scale>(0.5)
                .Add<Translate>(
                    i * dist - dist * gridSize/2,
                    j * dist - dist * gridSize/2,
                    2.0)
                // .Add<DynRotate>(0.0, 20.0, v3(0.0, 0.0, 1.0))
                .Compose();

            auto entity = MakeRef<Entity>(sphereModel, transform);
            GetEntityManager().AddEntity(entity);

            AddLight(new PointLight(v4(1.0f), v3(
                    i * dist - dist * gridSize/2,
                    j * dist - dist * gridSize/2,
                    1.0)));

            // parentEnt = entity.get();
        }
    }

}

void SpheresScene::OnUpdate(Timestep& ts) {
    Scene::OnUpdate(ts);
    m_CameraController.OnUpdate(ts);
}

void SpheresScene::OnEvent(Event& event) {
    m_CameraController.OnEvent(event);
    Scene::OnEvent(event);
}

void SpheresScene::OnImGuiRender() {
    auto p = GetCamera().GetPosition();
    static float camPos[3] = {0.f};
    camPos[0] = p.x;
    camPos[1] = p.y;
    camPos[2] = p.z;

    ImGui::Begin("Camera");
        ImGui::DragFloat3("Camera position", camPos, -20.f, 20.f);
        GetCamera().SetPosition(v3(camPos[0], camPos[1], camPos[2]));
    ImGui::End();

    if (ImGui::Begin("Entities")) {
        auto& entities = GetEntityManager().GetEntities();
        for (auto& ent : entities) {
            ImGui::PushID(ent.get());

            if (ImGui::CollapsingHeader("##Entity")) {
                auto& transform = ent->GetTransformMatrix();
                v3 pos = transform[3];

                if (ImGui::InputFloat3("Position", glm::value_ptr(pos))) {
                    // transform.SetPosition(pos); // update actual transform
                }

                auto model = ent->GetModel();
                auto material = model->GetMeshes()[0]->GetMaterial();

                if (material) {
                    ImGui::ColorEdit4("Albedo", glm::value_ptr(material->m_Albedo));
                    ImGui::SliderFloat("Metallic", &material->m_Metallic, 0.001f, 1.0f);
                    ImGui::SliderFloat("Roughness", &material->m_Roughness, 0.001f, 1.0f);
                }
            }

            ImGui::PopID();

        }
    }
    ImGui::End();
}

} // BallsSceneNS
