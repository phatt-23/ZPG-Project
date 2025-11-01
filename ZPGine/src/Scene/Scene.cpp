#include "Scene.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "SceneContext.h"
#include "Renderer/RenderCommand.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "Core/Layer.h"
#include "Debug/Logger.h"

#include "Event/WindowEvent.h"
#include "Renderer/Renderer.h"

#include "Core/Timestep.h"
#include "Debug/Asserter.h"
#include "Light/Light.h"

#include "Entity/Entity.h"
#include "Transform/StaticTransform/Rotate.h"
#include "Transform/StaticTransform/Scale.h"
#include "Transform/StaticTransform/Translate.h"
#include "Transform/TransformGroup.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Material/Material.h"
#include "imgui.h"
#include "Camera/CameraController.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Texture/Texture.h"

namespace ZPG {

Scene::Scene(const ref<ResourceManager>& resourceManager)
: m_ResourceManager(resourceManager) {
    m_CameraController = MakeRef(new CameraController(m_Camera));
}

Scene::~Scene() {
}

/**
 * Lifetime hook methods.
 */

void Scene::OnDetach() {
}

void Scene::OnLazyAttach() {
}

void Scene::OnAttach() {
    OnResume();
}

void Scene::OnResume() {
}

void Scene::OnPause() {
}

void Scene::OnUpdate(Timestep& ts) {
    SceneContext ctx = {
        .Ts = ts,
        .AddLight = [this](const Light& light) {
            ZPG_CORE_DEBUG("Layer wants to add light.");
            return;
        },
    };

    for (auto& layer : m_LayerStack) {
        layer->OnUpdate(ctx);
    }

    for (auto& entity : m_EntityManager.GetEntities()) {
        entity->Update(ts);
    }

    m_CameraController->OnUpdate(ts);
}

void Scene::OnRender(Timestep& ts) {
    RenderContext context = {
        .Ts = ts,
        .Cam = m_Camera,
        .Lights = m_LightManager.GetLights(),
    };

    Renderer::BeginDraw(m_Camera);
    Renderer::SetLights(m_LightManager.GetLights());
    {
        for (auto& layer : m_LayerStack) {
            layer->OnRender(context);
        }

        for (const auto& entity : m_EntityManager.GetEntities()) {
            Renderer::SubmitEntity(entity.get());
        }
    }
    Renderer::EndDraw();
}

void Scene::OnEvent(Event& event) {
    PropagateEventDownLayers(event);
    m_CameraController->OnEvent(event);
}

void Scene::OnImGuiRender() {
    ImGuiRenderEachLayer();
    ImGuiRenderDebug();
}

/**
 * Layering.
 */


void Scene::PushLayer(Layer* layer) {
    layer->SetScene(this);
    m_LayerStack.PushLayer(layer);
}

void Scene::PopLayer() {
    ref<Layer> layer = m_LayerStack.PopLayer();
    layer->SetScene(nullptr);
}

/**
 * Lighting.
 */

void Scene::AddLight(Light* light) {
    m_LightManager.AddLight(MakeRef(light));
}

void Scene::AddLight(const ref<Light>& light) {
    m_LightManager.AddLight(light);
}

void Scene::RemoveLight(Light* light) {
    m_LightManager.RemoveLight(MakeRef(light));
}

void Scene::AddEntity(Entity* entity) {
    m_EntityManager.AddEntity(MakeRef(entity));
}

void Scene::AddEntity(const ref<Entity>& entity) {
    m_EntityManager.AddEntity(entity);
}

void Scene::RemoveEntity(Entity* entity) {
    ZPG_NOT_IMPL();
}

const ref<CameraController>& Scene::GetCameraController() const {
    return m_CameraController;
}

void Scene::SetCameraController(const ref<CameraController>& cameraController) {
    m_CameraController = cameraController;
}

/**
 * Layering workflow helpers.
 */

void Scene::PropagateEventDownLayers(Event& event) {
    // events travel from top to bottom
    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(event);
        // if this layer has handled the event, don't propagate the event further down the layers
        if (event.IsHandled())  
            break;
    }
}

void Scene::ImGuiRenderEachLayer() {
    for (auto& layer : m_LayerStack) {
        layer->OnImGuiRender();
    }
}


/**
 * Entities helper methods.
 */


void TransformImGuiTreeNode(Transform* transform) {
    ImGui::PushID(transform);

    if (typeid(*transform) == typeid(TransformGroup)) {
        auto* group = (TransformGroup*)transform;
        auto& children = group->GetChildren();

        if (ImGui::TreeNodeEx("TransformGroup")) {
            for (int i = 0; i < children.size(); i++) {
                TransformImGuiTreeNode(children[i].get());
            }
            ImGui::TreePop();
        }
    }
    else if (typeid(*transform) == typeid(Translate)) {
        auto* translate = (Translate*)transform;
        v3 translation = translate->GetTranslation();

        ImGui::Text("Translate");
        if (ImGui::InputFloat3("Translation", glm::value_ptr(translation))) {
            translate->SetTranslation(translation);
        }
    }
    else if (typeid(*transform) == typeid(Rotate)) {
        auto* rotate = (Rotate*)transform;
        qtr rotation = rotate->GetRotation();

        ImGui::Text("Rotate");
        if (ImGui::InputFloat3("RotationQuat", glm::value_ptr(rotation))) {
            rotate->SetRotation(rotation);
        }
    }
    else if (typeid(*transform) == typeid(Scale)) {
        auto* scale = (Scale*)transform;
        v3 scaleVec = scale->GetScale();

        ImGui::Text("Scale");
        if (ImGui::InputFloat3("Scale", glm::value_ptr(scaleVec))) {
            scale->SetScale(scaleVec);
        }
    }

    ImGui::PopID();
}

void TextureImGuiImage(Texture* texture) {
    ImGui::PushID(texture);
    if (ImGui::TreeNodeEx(texture->GetName().c_str())) {
        ImVec2 imageSize(200, 200);
        auto* glTexture = (OpenGLTexture*)texture;
        ImGui::Image(glTexture->m_RendererID, imageSize);

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void ModelImGuiTreeNode(Model* model) {
    ImGui::PushID(model);
    
    ImGui::Text("Model");

    auto& meshes = model->GetMeshes();
    for (int i = 0; i < meshes.size(); i++) {
        auto& mesh = meshes[i];
        ImGui::PushID(mesh.get());
        if (ImGui::TreeNodeEx("Mesh")) {

            auto& material = mesh->GetMaterial();

            ImGui::Text("Material");

            v4 albedo = material->GetAlbedo();
            if (ImGui::ColorPicker4("Albedo", glm::value_ptr(albedo))) {
                material->SetAlbedo(albedo);
            }

            v4 emissive = material->GetEmissive();
            if (ImGui::ColorPicker4("Emissive", glm::value_ptr(emissive))) {
                material->SetEmissive(emissive);
            }

            f32 metallic = material->GetMetallic();
            if (ImGui::SliderFloat("Metallic", &metallic, 0.0, 1.0)) {
                material->SetMetallic(metallic);
            }

            f32 roughness = material->GetRoughness();
            if (ImGui::SliderFloat("Roughness", &roughness, 0.0, 1.0)) {
                material->SetRoughness(roughness);
            }

            TextureImGuiImage(material->GetAlbedoMap().get());
            TextureImGuiImage(material->GetNormalMap().get());
            TextureImGuiImage(material->GetMetalnessMap().get());
            TextureImGuiImage(material->GetRoughnessMap().get());
            TextureImGuiImage(material->GetEmissiveMap().get());

            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    ImGui::PopID();
}

void Scene::ImGuiRenderDebug() {
    ImGui::Begin("Entities");

    auto entities = m_EntityManager.GetEntities();

    for (int i = 0; i < entities.size(); i++) {
        ref<Entity> ent = entities[i];
        ImGui::PushID(ent.get());

        if(ImGui::TreeNodeEx(ent->GetModel()->GetName().c_str())) {
            auto& transform = ent->GetTransform();
            auto& model = ent->GetModel();

            TransformImGuiTreeNode(transform.get());

            ModelImGuiTreeNode(model.get());

            ImGui::TreePop(); 
        }

        ImGui::PopID();
    }
    ImGui::End();

    ImGui::Begin("Lights");
    auto lights = m_LightManager.GetLights();
    for (auto& i : lights) {
        Light* lightPtr = i.get();
        LightType lightType = lightPtr->GetLightType();

        ImGui::PushID(lightPtr);
        if (lightType == LightType::Ambient && ImGui::TreeNodeEx("AmbientLight")) {
            auto light = (AmbientLight*)lightPtr;

            v4 color = light->Color.Get();
            if (ImGui::ColorPicker4("Color", glm::value_ptr(color))) {
                light->Color.Set(color);
            }

            ImGui::TreePop();
        }
        else if (lightType == LightType::Directional && ImGui::TreeNodeEx("DirectionalLight")) {
            auto light = (DirectionalLight*)lightPtr;

            v4 color = light->Color.Get();
            if (ImGui::ColorPicker4("Color", glm::value_ptr(color))) {
                light->Color.Set(color);
            }

            v3 dir = light->Direction.Get();
            if (ImGui::InputFloat3("Direction", glm::value_ptr(dir))) {
                light->Direction.Set(dir);
            }

            ImGui::TreePop();
        }
        else if (lightType == LightType::Point && ImGui::TreeNodeEx("PointLight")) {
            auto light = (PointLight*)lightPtr;

            v4 color = light->Color.Get();
            if (ImGui::ColorPicker4("Color", glm::value_ptr(color))) {
                light->Color.Set(color);
            }

            v3 pos = light->Position.Get();
            if (ImGui::InputFloat3("Position", glm::value_ptr(pos))) {
                light->Position.Set(pos);
            }

            v3 atten = light->Atten.GetAttenuation();
            if (ImGui::InputFloat3("Attenuation", glm::value_ptr(atten))) {
                light->Atten.SetAttenuation(atten);
            }

            ImGui::TreePop();
        }
        else if (lightType == LightType::Spotlight && ImGui::TreeNodeEx("SpotLight")) {
            auto light = (SpotLight*)lightPtr;

            v4 color = light->Color.Get();
            if (ImGui::ColorPicker4("Color", glm::value_ptr(color))) {
                light->Color.Set(color);
            }

            v3 pos = light->Position.Get();
            if (ImGui::InputFloat3("Position", glm::value_ptr(pos))) {
                light->Position.Set(pos);
            }

            v3 dir = light->Direction.Get();
            if (ImGui::InputFloat3("Direction", glm::value_ptr(dir))) {
                light->Direction.Set(dir);
            }

            v3 atten = light->Atten.GetAttenuation();
            if (ImGui::DragFloat3("Attenuation", glm::value_ptr(atten), 0.01)) {
                light->Atten.SetAttenuation(atten);
            }

            f32 beamSize = light->BeamShape.GetSize();
            if (ImGui::SliderFloat("BeamSize", &beamSize, 0.0, 180.0)) {
                light->BeamShape.SetSize(beamSize);
            }

            f32 beamBlend = light->BeamShape.GetBlend();
            if (ImGui::SliderFloat("BeamBlend", &beamBlend, 0.0, 1.0)) {
                light->BeamShape.SetBlend(beamBlend);
            }

            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    ImGui::End();
}


} // namespace ZPG
