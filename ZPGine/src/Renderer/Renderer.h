//
// Created by phatt on 9/22/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "DrawData.h"
#include "RendererAPI.h"
#include "Light/LightManager.h"

namespace ZPG {

struct DrawData;
class Model;
class ShaderProgram;
class Mesh;
class Entity;
class Scene;
class Light;
class Camera;
class Material;
class RenderStatistics;

class Renderer {
public:
    static void Init();  // must be called explicitly during Application initialization
    static void Shutdown();

    static void BeginDraw(const Camera& camera);
    static void EndDraw();

    static void SetLights(const std::vector<ref<Light>>& lights);

    /**
     * Render a whole scene. Pulls the drawable objects from the scene, the lights and camera.
     * Can optimize by grouping the objects by group-by('shaderProgram', 'material').
     * Instantiated draw can draw entities that are instances of the same model.
     * Objects that share shaderPrograms are drawn together to reduce the communication
     * between CPU and GPU since data bus operations are the most expensive. */
    static void RenderScene(const Scene& scene);

    static void SubmitEntity(const Entity* entity, const m4& transform = m4(1.0f));
    static void SubmitModel(const Model* model, const m4& transform = m4(1.0f));
    static void SubmitMesh(const Mesh* mesh, const m4& transform = m4(1.0f));

    static void Flush();
    static RendererAPI::API GetAPI();

    static void OnWindowResize(int width, int height);

    static void SetInstanced(bool enabled) { s_Instanced = enabled; }
    static bool IsInstanced() { return s_Instanced; }

    static RenderStatistics& GetStats() { return *s_Stats; }

private:
    inline static DrawData* s_DrawData = nullptr;
    inline static bool s_Instanced = true;

    inline static RenderStatistics* s_Stats = nullptr;
};


}

#endif //RENDERER_H
