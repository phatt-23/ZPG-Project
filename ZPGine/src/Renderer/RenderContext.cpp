#include "RenderContext.h"
#include "RenderBindingPoints.h"
#include "Profiling/Instrumentor.h"
#include "Sky/Skybox.h"

namespace ZPG 
{
    RenderContext::RenderContext(const RenderContextSpecification& spec)
        : CameraSSBO(RenderBindingPoints::CAMERA_SSBO)
        , EnvironmentLightSSBO(RenderBindingPoints::ENVIRONMENT_LIGHT_SSBO)
        , PointLightSSBO(RenderBindingPoints::POINT_LIGHT_SSBO, spec.PointLightCapacity)
        , SpotLightSSBO(RenderBindingPoints::SPOT_LIGHT_SSBO, spec.SpotLightCapacity)
        , MaterialSSBO(RenderBindingPoints::MATERIAL_SSBO)
        , ModelSSBO(RenderBindingPoints::MODELS_SSBO, spec.BatchSize)
        , EntitySSBO(RenderBindingPoints::ENTITY_SSBO, spec.BatchSize)
        , Batch(spec.BatchSize)
    {
        ZPG_PROFILE_FUNCTION();
        SpotLights.reserve(spec.SpotLightCapacity);
        PointLights.reserve(spec.PointLightCapacity);


        FrameBufferSpecification directionalLightFramebufferSpec;
        directionalLightFramebufferSpec.Width = 4 * 1024;
        directionalLightFramebufferSpec.Height = 4 * 1024;
        directionalLightFramebufferSpec.Resizable = false;
        directionalLightFramebufferSpec.Attachments = {
            {TextureDataFormat::Depth32F, 0},
            {TextureDataFormat::RGBA32F, 0}
        };

        DirectionalLightShadowFramebuffer = FrameBuffer::Create(directionalLightFramebufferSpec);


        FrameBufferSpecification pointLightShadowFramebufferSpec;
        pointLightShadowFramebufferSpec.Width = 1024;
        pointLightShadowFramebufferSpec.Height = 1024;
        pointLightShadowFramebufferSpec.Resizable = false;
        pointLightShadowFramebufferSpec.Attachments = {
            {TextureDataFormat::Depth32F, 0},
        };

        PointLightShadowFramebuffer = FrameBuffer::Create(pointLightShadowFramebufferSpec);


        FrameBufferSpecification spotLightShadowFramebufferSpec;
        spotLightShadowFramebufferSpec.Width = 1024;
        spotLightShadowFramebufferSpec.Height = 1024;
        spotLightShadowFramebufferSpec.Resizable = false;
        spotLightShadowFramebufferSpec.Attachments = {
            {TextureDataFormat::Depth32F, 0},
            // {TextureDataFormat::RGBA32F, 0}, // debug depth
        };

        SpotLightShadowFramebuffer = FrameBuffer::Create(spotLightShadowFramebufferSpec);


        FrameBufferSpecification GeometryPassFramebufferSpec;
        GeometryPassFramebufferSpec.Width = 1024;
        GeometryPassFramebufferSpec.Height = 1024;
        GeometryPassFramebufferSpec.Resizable = true;
        GeometryPassFramebufferSpec.Attachments = {
            { TextureDataFormat::Depth24Stencil8, 0 },
            { TextureDataFormat::RGBA32F, 0 }, // pos
            { TextureDataFormat::RGBA32F, 1 }, // normal
            { TextureDataFormat::RGBA32F, 2 }, // albedo metallic
            { TextureDataFormat::RGBA32F, 3 }, // emissive roughness
            { TextureDataFormat::RedInteger, 4 }, // entity id
            { TextureDataFormat::RGBA8, 5 }, // entity id colorized
        };

        GeometryPassFramebuffer = FrameBuffer::Create(GeometryPassFramebufferSpec);


        FrameBufferSpecification mainFramebufferSpec;
        mainFramebufferSpec.Width = 1024;
        mainFramebufferSpec.Height = 1024;
        mainFramebufferSpec.Resizable = true;
        mainFramebufferSpec.Attachments = {
            {TextureDataFormat::RGBA8, 0},      // viewing result
            {TextureDataFormat::RedInteger, 1}, // entity id
            {TextureDataFormat::RGBA8, 2}, // entity id colorized
            {TextureDataFormat::Depth24Stencil8, 0},
        };

        MainFramebuffer = FrameBuffer::Create(mainFramebufferSpec);


        ActiveSky = Skybox::Create(SkyboxSpecification{ .Directory = "./assets/textures/basic-skybox/" });
    }

    RenderContext::~RenderContext()
    {
        ZPG_PROFILE_FUNCTION();
    }
}
