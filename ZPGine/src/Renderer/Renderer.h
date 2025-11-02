//
// Created by phatt on 9/22/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "RendererAPI.h"

namespace ZPG {
class Skybox;

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
    static RendererAPI::API GetAPI();

    static void Init();  // must be called explicitly during Application initialization
    static void Shutdown();

    static void BeginDraw(const Camera& camera);
    static void EndDraw();
    static void Flush();

    static void SetLights(const std::vector<ref<Light>>& lights);
    static void SetSkybox(const ref<Skybox>& skybox);

    static void SubmitEntity(const Entity* entity, const m4& transform = m4(1.0f));
    static void SubmitModel(const Model* model, const m4& transform = m4(1.0f), int entityID = -1);
    static void SubmitMesh(const Mesh* mesh, const m4& transform = m4(1.0f), int entityID = -1);

    static void SetInstanced(bool enabled) { s_Instanced = enabled; }
    static bool IsInstanced() { return s_Instanced; }

    static void SetDeferred(bool enabled) { s_Deferred = enabled; }
    static bool IsDeferred() { return s_Deferred; }

    static RenderStatistics& GetStats() { return *s_Stats; }
    static DrawData& GetDrawData() { return *s_DrawData; }

    static void OnWindowResize(int width, int height);
    static void OnViewportResize(int width, int height);

private:
    static void InstancedRender();
    static void NonInstancedRender();

    static void BeginDeferred();
    static void EndDeferred();

private:
    inline static DrawData* s_DrawData = nullptr;
    inline static bool s_Instanced = true;
    inline static bool s_Deferred = true;

    inline static RenderStatistics* s_Stats = nullptr;
};


}

#endif //RENDERER_H
