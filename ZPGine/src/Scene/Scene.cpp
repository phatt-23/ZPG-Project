#include "Scene.h"

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
#include "Transform/TransformGroup.h"
#include "Model/Model.h"
#include "Model/Mesh.h"
#include "Material/Material.h"

namespace ZPG {

Scene::Scene(const ref<ResourceManager>& resourceManager)
: m_ResourceManager(resourceManager) {

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
    // Sending event because the scene must adapt to the current window size 
    const Window& window = Application::Get().GetWindow(); 
    WindowResizeEvent event(window.GetWidth(), window.GetHeight());
    this->OnEvent(event);
}

void Scene::OnPause() {
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

void Scene::RemoveEntity(Entity* entity) {
    ZPG_NOT_IMPL();
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

void Scene::UpdateEachLayer(Timestep& ts) {
    RenderCommand::SetClearColor({0.0, 0.0, 0.0, 1.0});
    RenderCommand::Clear();

    SceneContext ctx;
    ctx.Ts = ts;
    ctx.AddLight = [this](const Light& light) {
        ZPG_CORE_DEBUG("Layer wants to add light.");
        return;  
    };

    for (auto& layer : m_LayerStack) {
        layer->OnUpdate(ctx);
    }
}

void Scene::RenderEachLayer(Timestep& ts) {
    RenderContext ctx = {
        .Ts = ts,
        .Cam = m_Camera,
        .Lights = m_LightManager.GetLights(),
    };

    Renderer::BeginDraw(m_Camera);
    Renderer::SetLights(m_LightManager.GetLights());
    for (auto& layer : m_LayerStack) {
        layer->OnRender(ctx);
    }
    Renderer::EndDraw();
}


/**
 * Entities helper methods.
 */

void Scene::RenderEntities(Timestep& ts) {
    Renderer::BeginDraw(m_Camera);
    Renderer::SetLights(m_LightManager.GetLights());
    for (const auto& entity : m_EntityManager.GetEntities()) {
        Renderer::SubmitEntity(entity.get());
    }
    Renderer::EndDraw();
}

void Scene::UpdateEntities(Timestep& ts) {
    for (auto& entity : m_EntityManager.GetEntities()) {
        entity->Update(ts);
    }
}

void Scene::ImGuiRenderDebug() {
    ImGui::Begin("Entities");

    for (auto& ent : m_EntityManager.GetEntities()) {
        ImGui::PushID(ent.get());

        if (ImGui::CollapsingHeader("##Entity")) {

            // Model
            const ref<Model>& model = ent->GetModel();

            ImGui::Text("model name: %s", model->GetName().c_str());

            ImGui::Text("Transform");

            // Meshes
            for (auto& mesh : model->GetMeshes()) {
                ImGui::PushID(mesh.get());
                ImGui::BeginChild("Mesh");

                // Material
                const ref<Material>& material = mesh->GetMaterial();

                v4 albedo = material->GetAlbedo();
                f32 metallic = material->GetMetallic();
                f32 roughness = material->GetRoughness();

                ImGui::Text("Name: %s", material->GetName().c_str());

                if (ImGui::ColorEdit4("Abledo", glm::value_ptr(albedo))) {
                    material->SetAlbedo(albedo);
                }

                if (ImGui::SliderFloat("Metallic", &metallic, 0.0, 1.0)) {
                    material->SetMetallic(metallic);
                }

                if (ImGui::SliderFloat("Roughness", &roughness, 0.0, 1.0)) {
                    material->SetRoughness(roughness);
                }

                ImGui::EndChild();
                ImGui::PopID();
            }

        }
        
        ImGui::PopID();
    }
    ImGui::End();
}


} // namespace ZPG
