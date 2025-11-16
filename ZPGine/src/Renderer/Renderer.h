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

    enum class RenderFeature : u32
    {
        CastsShadow = ZPG_BIT(1),
        Forward     = ZPG_BIT(2),
        Deferred    = ZPG_BIT(3),
        Opaque      = ZPG_BIT(4),
        Transparent = Forward | ZPG_BIT(5),
    };

    inline u32 operator|(RenderFeature a, RenderFeature b)
    {
        return (u32)a | (u32)b;
    }

    class RenderFlags 
    {
    public:
        u32 Flags = 0;

        RenderFlags() = default;
        RenderFlags(u32 flags) : Flags(flags) {}
        RenderFlags(RenderFeature feature) : Flags((u32)feature) {}

        bool IsValid() {
            // either transparent, or opaque
            bool c1 = !((*this & RenderFeature::Transparent) && (*this & RenderFeature::Opaque));
            // if deferred, cannot be transparent
            bool c2 = !((*this & RenderFeature::Deferred) && (*this & RenderFeature::Transparent));
            // must be either forward or deferred
            bool c3 = (*this & RenderFeature::Forward) ^ (*this & RenderFeature::Deferred);

            return c1 && c2 && c3;
        }

        bool IsSet(RenderFeature feature) { return Flags & (u32)feature; }
        bool operator==(RenderFlags other) { return Flags == other.Flags; }
        bool operator&(RenderFeature feature) { return IsSet(feature); }

        RenderFlags& operator|=(u32 flags) 
        {
            Flags |= flags;
            return *this;
        }

        RenderFlags& operator|=(RenderFeature feature) 
        {
            Flags |= (u32)feature;
            return *this;
        }
    };

    class Renderer 
    {
    public:
        static void NewFrame();
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
