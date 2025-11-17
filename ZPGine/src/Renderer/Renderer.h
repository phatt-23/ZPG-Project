#pragma once

#include "RenderFlags.h"
#include "RenderStatistics.h"
#include "Renderer/RenderContext.h"

namespace ZPG
{
    class RenderPass;
    class Camera;
    class Entity;
    class Model;
    class Mesh;
    class Scene;

    class Renderer 
    {
    public:
        static void BeginFrame();
        static void SetCamera(const Camera& camera);
        static void SetLights(const std::vector<ref<Light>>& lights);
        static void SetSky(const Sky* sky);
        static void Submit(const Entity& entity, RenderFlags flags);
        static void EndFrame();

        static void RenderScene(const Scene& scene);

        static void Init();
        static void Shutdown();
        static void PushRenderPass(RenderPass* renderPass);

        static const RenderContext& GetRenderContext();
        static const RenderStatistics& GetStats();

        static void Clear();

        static void OnResize(u32 width, u32 height);

    private:
        Renderer(const RenderContextSpecification& renderContextSpecification);
        ~Renderer();

    private:
        RenderContext m_RenderContext;
        std::vector<RenderPass*> m_RenderPasses;

        inline static Renderer* s = nullptr;
    };
}
