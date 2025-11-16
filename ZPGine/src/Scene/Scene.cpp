#include "Scene.h"

#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"
#include "SceneContext.h"
#include "Core/Layer.h"
#include "Debug/Logger.h"

#include "Event/WindowEvent.h"

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
#include "Profiling/Instrumentor.h"
#include "Texture/Texture2D.h"
#include "Transform/DynamicTransform/DynRotate.h"
#include "Transform/DynamicTransform/DynScale.h"
#include "Transform/DynamicTransform/DynTranslate.h"

namespace ZPG {

Scene::Scene(const ref<ResourceManager>& resourceManager)
    : m_ResourceManager(resourceManager)
{
    ZPG_PROFILE_FUNCTION();
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
    ZPG_PROFILE_FUNCTION();
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

#if 0
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
#endif

void Scene::OnEvent(Event& event) {
    ZPG_PROFILE_FUNCTION();
    PropagateEventDownLayers(event);
    m_CameraController->OnEvent(event);
}

void Scene::OnImGuiRender() {
    ZPG_PROFILE_FUNCTION();
    ImGuiRenderEachLayer();
    ImGuiRenderDebug();
}

/**
 * Layering.
 */


void Scene::PushLayer(Layer* layer) {
    ZPG_PROFILE_FUNCTION();
    layer->SetScene(this);
    m_LayerStack.PushLayer(layer);
}

void Scene::PopLayer() {
    ZPG_PROFILE_FUNCTION();
    ref<Layer> layer = m_LayerStack.PopLayer();
    layer->SetScene(nullptr);
}

/**
 * Lighting.
 */

void Scene::AddLight(Light* light) {
    ZPG_PROFILE_FUNCTION();
    m_LightManager.AddLight(MakeRef(light));
}

void Scene::AddLight(const ref<Light>& light) {
    ZPG_PROFILE_FUNCTION();
    m_LightManager.AddLight(light);
}

void Scene::RemoveLight(Light* light) {
    ZPG_PROFILE_FUNCTION();
    m_LightManager.RemoveLight(MakeRef(light));
}

void Scene::AddEntity(Entity* entity) {
    ZPG_PROFILE_FUNCTION();
    m_EntityManager.AddEntity(MakeRef(entity));
}

void Scene::AddEntity(const ref<Entity>& entity) {
    ZPG_PROFILE_FUNCTION();
    m_EntityManager.AddEntity(entity);
}

void Scene::RemoveEntity(Entity* entity) {
    ZPG_PROFILE_FUNCTION();
    ZPG_NOT_IMPL();
}

const ref<CameraController>& Scene::GetCameraController() const {
    ZPG_PROFILE_FUNCTION();
    return m_CameraController;
}

void Scene::SetCameraController(const ref<CameraController>& cameraController) {
    ZPG_PROFILE_FUNCTION();
    m_CameraController = cameraController;
}

void Scene::SetSky(const ref<Sky>& sky) {
    ZPG_PROFILE_FUNCTION();
    m_Sky = sky;
}

const ref<Sky>& Scene::GetSky() const {
    ZPG_PROFILE_FUNCTION();
    return m_Sky;
}

/**
 * Layering workflow helpers.
 */

void Scene::PropagateEventDownLayers(Event& event) {
    ZPG_PROFILE_FUNCTION();
    // events travel from top to bottom
    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(event);
        // if this layer has handled the event, don't propagate the event further down the layers
        if (event.IsHandled())  
            break;
    }
}

void Scene::ImGuiRenderEachLayer() {
    ZPG_PROFILE_FUNCTION();
    for (auto& layer : m_LayerStack) {
        layer->OnImGuiRender();
    }
}


/**
 * User interface
 */

void TransformImGuiTreeNode(Transform* transform) {
    ZPG_PROFILE_FUNCTION();
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
        if (ImGui::DragFloat3("Translation", glm::value_ptr(translation), 0.01f)) {
            translate->SetTranslation(translation);
        }
    }
    else if (typeid(*transform) == typeid(Rotate)) {
        auto* rotate = (Rotate*)transform;
        qtr rotation = rotate->GetRotation();

        ImGui::Text("Rotate");
        if (ImGui::DragFloat4("RotationQuat", glm::value_ptr(rotation), 0.01f, -1.0, 1.0)) {
            rotate->SetRotation(rotation);
        }
    }
    else if (typeid(*transform) == typeid(Scale)) {
        auto* scale = (Scale*)transform;
        v3 scaleVec = scale->GetScale();

        ImGui::Text("Scale");
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scaleVec), 0.01f)) {
            scale->SetScale(scaleVec);
        }
    }
    else if (typeid(*transform) == typeid(DynRotate)) {
        auto* dynRotate = (DynRotate*)transform;

        v3 rotationAxis = dynRotate->GetRotationAxis();
        f32 currentRotation = dynRotate->GetCurrentRotationDeg();
        f32 rotationSpeed = dynRotate->GetRotationSpeedDeg();

        ImGui::Text("DynRotate");
        ImGui::Text("Current Rotation: %f", currentRotation);

        if (ImGui::DragFloat3("Rotation Axis", glm::value_ptr(rotationAxis), 0.01f, 0.0, 1.0)) {
            dynRotate->SetRotationAxis(rotationAxis);
        }

        if (ImGui::DragFloat("Rotation Speed", &rotationSpeed, 0.01f, 0.0, 100.0)) {
            dynRotate->SetRotationSpeedDeg(rotationSpeed);
        }
    }
    else if (typeid(*transform) == typeid(DynScale)) {
        auto* dynScale = (DynScale*)transform;

        v3 growth = dynScale->GetGrowth();
        v3 min = dynScale->GetMinScale();
        v3 max = dynScale->GetMaxScale();
        v3 current = dynScale->GetCurrentScale();

        ImGui::Text("DynScale");
        ImGui::Text("Current Scale: %f", current.x);
        if (ImGui::DragFloat3("Growth", glm::value_ptr(growth), 0.01f, 0.0, 100.0)) {
            dynScale->SetGrowth(growth);
        }
        if (ImGui::DragFloat3("Min Scale", glm::value_ptr(min), 0.01f, 0.0, 100.0)) {
            dynScale->SetMinScale(min);
        }
        if (ImGui::DragFloat3("Max Scale", glm::value_ptr(max), 0.01f, 0.0, 100.0)) {
            dynScale->SetMaxScale(max);
        }
    }
    else if (typeid(*transform) == typeid(DynTranslate)) {
        auto* dynTranslate = (DynTranslate*)transform;
        v3 current = dynTranslate->GetCurrentTranslation();
        v3 inc = dynTranslate->GetTranslationIncrement();
        v3 max = dynTranslate->GetMaxTranslation();
        v3 min = dynTranslate->GetMinTranslation();

        ImGui::Text("DynTranslate");
        ImGui::Text("Current Translation: %f", current.x);
        if (ImGui::DragFloat3("Translation Increment", glm::value_ptr(inc), 0.01f, 0.0, 100.0)) {
            dynTranslate->SetTranslationIncrement(inc);
        }
        if (ImGui::DragFloat3("Max Translation", glm::value_ptr(max), 0.01f, 0.0, 100.0)) {
            dynTranslate->SetMaxTranslation(max);
        }
        if (ImGui::DragFloat3("Min Translation", glm::value_ptr(min), 0.01f, 0.0, 100.0)) {
            dynTranslate->SetMinTranslation(min);
        }
    }

    ImGui::PopID();
}


void TextureImGuiImage(Texture2D* texture) {
    ZPG_PROFILE_FUNCTION();
    ImGui::PushID(texture);
    if (ImGui::TreeNodeEx(texture->GetName().c_str())) {
        ImVec2 imageSize(200, 200);
        auto* glTexture = (OpenGLTexture2D*)texture;
        ImGui::Image(glTexture->GetRendererID(), imageSize);

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void ModelImGuiTreeNode(Model* model) {
    ZPG_PROFILE_FUNCTION();
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
    ZPG_PROFILE_FUNCTION();
    ImGui::Begin("Entities");

    auto entities = m_EntityManager.GetEntities();

    for (int i = 0; i < entities.size(); i++) {
        ref<Entity> ent = entities[i];
        ImGui::PushID(ent.get());

        std::string label = ent->GetModel()->GetName() + " #" + std::to_string(ent->GetEntityID());

        if(ImGui::TreeNodeEx(label.c_str())) {
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
            if (ImGui::DragFloat3("Direction", glm::value_ptr(dir), 0.01, -1.0, 1.0)) {
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
            if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.01)) {
                light->Position.Set(pos);
            }

            v3 atten = light->Atten.GetAttenuation();
            if (ImGui::DragFloat3("Attenuation", glm::value_ptr(atten), 0.01, 0.0, 1.0)) {
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
            if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.01)) {
                light->Position.Set(pos);
            }

            v3 dir = light->Direction.Get();
            if (ImGui::DragFloat3("Direction", glm::value_ptr(dir), 0.01, -1.0, 1.0)) {
                light->Direction.Set(dir);
            }

            v3 atten = light->Atten.GetAttenuation();
            if (ImGui::DragFloat3("Attenuation", glm::value_ptr(atten), 0.01, 0.0, 1.0)) {
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
