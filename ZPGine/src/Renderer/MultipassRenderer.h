#pragma once

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

    class MultipassRenderer
    {
    public:
        static void Init();
        static void Shutdown();
        static void PushRenderPass(RenderPass* renderPass);
        static void RenderScene(const Scene& scene);
        static void OnResize(u32 width, u32 height);
        static const RenderContext& GetRenderContext();
        static const RenderStatistics& GetStats();

    private:
        MultipassRenderer(const RenderContextSpecification& renderContextSpecification);
        ~MultipassRenderer();

        void PushRenderPassImpl(RenderPass* renderPass);
        void RenderSceneImpl(const Scene& scene);
        void OnResizeImpl(u32 width, u32 height);
        void SetLightsImpl(const std::vector<ref<Light>>& lights);
        void SetCameraImpl(const Camera& camera);

    private:
        RenderContext m_RenderContext;

        std::vector<RenderPass*> m_RenderPasses;

        inline static MultipassRenderer* s_Instance = nullptr;
    };

/*
        
        sets up its RenderContext
        (it will inject this RenderContext into its renderPasses)

 
        MultipassRenderer::Init();

        RenderContext has:
            - ActiveCamera
            - DirectionalLight   - these are kept separate because the shader expects them separate
            - AmbientLight
            - PointLights
            - SpotLights
            - RenderBatch       - merges draw calls, so the total number of draw calls is reduced as mush as possible
                - DrawCommands 

                // possibly in the future
                - DeferredDrawCommands  - drawn into the g-buffer during the deferred pass
                - ForwardDrawCommands  - these have special requirement for shading, or are transparent and need to be draw in the forward pass

...

        MultipassRenderer::SubmitEntity(entity_1)   // pushes a draw call(s) into the RenderBatch
        MultipassRenderer::SubmitEntity(entity_2)
        ...
        MultipassRenderer::SubmitEntity(entity_n)


        when renderBatch is filled up, the batch is drawn and flushed

        MultipassRenderer::Flush()  // either called by client or internaly when the batch is full


...
        deallocates its members, draw data, ...

        MultipassRenderer::Shutdown();
*/

}
