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
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
            },
            { .DataFormat = TextureDataFormat::RGBA32F, .Index = 0, }
        };

        DirectionalLightShadowFramebuffer = FrameBuffer::Create(directionalLightFramebufferSpec);


        FrameBufferSpecification pointLightShadowFramebufferSpec;
        pointLightShadowFramebufferSpec.Width = 1024;
        pointLightShadowFramebufferSpec.Height = 1024;
        pointLightShadowFramebufferSpec.Resizable = false;
        pointLightShadowFramebufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .TexType = TextureType::TextureCubeMapArray,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0
            },
        };

        PointLightShadowFramebuffer = FrameBuffer::Create(pointLightShadowFramebufferSpec);


        FrameBufferSpecification spotLightShadowFramebufferSpec;
        spotLightShadowFramebufferSpec.Width = 1024;
        spotLightShadowFramebufferSpec.Height = 1024;
        spotLightShadowFramebufferSpec.Resizable = false;
        spotLightShadowFramebufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .TexType = TextureType::Texture2DArray,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
            },
        };

        SpotLightShadowFramebuffer = FrameBuffer::Create(spotLightShadowFramebufferSpec);


        FrameBufferSpecification geometryPassFramebufferSpec;
        geometryPassFramebufferSpec.Width = 1024;
        geometryPassFramebufferSpec.Height = 1024;
        geometryPassFramebufferSpec.Resizable = true;
        geometryPassFramebufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::DepthStencil,
                .DataFormat = TextureDataFormat::Depth24Stencil8,
                .Index = 0,
            },
            { .DataFormat = TextureDataFormat::RGBA32F, .Index = 0, }, // pos
            { .DataFormat = TextureDataFormat::RGBA32F, .Index = 1, }, // normal
            { .DataFormat = TextureDataFormat::RGBA32F, .Index = 2, }, // albedo metallic
            { .DataFormat = TextureDataFormat::RGBA32F, .Index = 3, }, // emissive roughness
            { .DataFormat = TextureDataFormat::RedInteger, .Index = 4, }, // entity id
        };

        GeometryPassFramebuffer = FrameBuffer::Create(geometryPassFramebufferSpec);


        FrameBufferSpecification mainFramebufferSpec;
        mainFramebufferSpec.Width = 1024;
        mainFramebufferSpec.Height = 1024;
        mainFramebufferSpec.Resizable = true;
        mainFramebufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::DepthStencil,
                .DataFormat = TextureDataFormat::Depth24Stencil8,
                .Index = 0
            },
            { .DataFormat = TextureDataFormat::RGBA8, .Index = 0 }, // viewing result
            { .DataFormat = TextureDataFormat::RedInteger, .Index = 1 }, // entity id
        };

        MainFramebuffer = FrameBuffer::Create(mainFramebufferSpec);
    }

    RenderContext::~RenderContext()
    {
        ZPG_PROFILE_FUNCTION();
    }
}
