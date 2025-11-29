#include "RenderContext.h"
#include "Debug/Logger.h"
#include "RenderBindingPoints.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/DrawBatch.h"
#include "Renderer/DrawCommand.h"
#include "Texture/Texture2D.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"
#include "Texture/TextureDataFormat.h"

namespace ZPG 
{
    RenderContext::RenderContext(const RenderContextSpecification& spec)
        : SSBO{
            .CameraSSBO = CameraShaderStorageBuffer(RenderBindingPoints::CAMERA_SSBO),
            .ModelSSBO = ModelShaderStorageBuffer(RenderBindingPoints::MODEL_SSBO, spec.BatchSize),
            .EntitySSBO = EntityShaderStorageBuffer(RenderBindingPoints::ENTITY_SSBO, spec.BatchSize),
            .EnvironmentLightSSBO = EnvironmentLightShaderStorageBuffer(RenderBindingPoints::ENVIRONMENT_LIGHT_SSBO),
            .PointLightSSBO = PointLightShaderStorageBuffer(RenderBindingPoints::POINTLIGHT_SSBO, spec.PointLightCapacity),
            .SpotLightSSBO = SpotLightShaderStorageBuffer(RenderBindingPoints::SPOTLIGHT_SSBO, spec.SpotLightCapacity),
            .MaterialSSBO = MaterialShaderStorageBuffer(RenderBindingPoints::MATERIAL_SSBO),
            .ProcessingSSBO = ProcessingShaderStorageBuffer(RenderBindingPoints::PROCESSING_SSBO),
        }
        , Targets{}
        , Queues{}
        , Batches{} 
        , Lights{}
    {
        ZPG_PROFILE_FUNCTION();
        Lights.SpotLights.reserve(spec.SpotLightCapacity);
        Lights.PointLights.reserve(spec.PointLightCapacity);

        u32 width = 1024;
        u32 height = 1024;

        Targets.GeometryDepthMap = Texture2D::Create("geometry_depth_map.texture2d", width, height, TextureDataFormat::Depth32F);
        Targets.GeometryPositionMap = Texture2D::Create("geometry_position_map.texture2d", width, height, TextureDataFormat::RGBA32F);
        Targets.GeometryNormalMap = Texture2D::Create("geometry_normal_map.texture2d", width, height, TextureDataFormat::RGBA32F);
        Targets.GeometryAlbedoMetallicMap = Texture2D::Create("geometry_albedo_metallic_map.texture2d", width, height, TextureDataFormat::RGBA32F);
        Targets.GeometryEmissiveRoughnessMap = Texture2D::Create("geometry_emissive_roughness_map.texture2d", width, height, TextureDataFormat::RGBA32F);
        Targets.GeometryEntityIDMap = Texture2D::Create("geometry_entity_id_map.texture2d", width, height, TextureDataFormat::RedInteger);

        Targets.MainDepthMap = Texture2D::Create("main_depth_map.texture2d", width, height, TextureDataFormat::Depth32F);
        Targets.MainColorMap = Texture2D::Create("main_color_map.texture2d", width, height, TextureDataFormat::RGBA8);
        Targets.MainEntityIDMap = Texture2D::Create("main_entity_id_map.texture2d", width, height, TextureDataFormat::RedInteger);

        Targets.SpotLightShadowMapArray = Texture2DArray::Create( 1024, 1024, 8, TextureDataFormat::Depth32F );
        Targets.PointLightShadowCubeMapArray = TextureCubeMapArray::Create( 1024, 8, TextureDataFormat::Depth32F );
        Targets.DirectionalLightShadowMapArray = Texture2DArray::Create( 1024 << 2, 1024 << 2, 4, TextureDataFormat::Depth32F );

        FrameBufferSpecification mainFramebufferSpec;
        mainFramebufferSpec.Width = 1024;
        mainFramebufferSpec.Height = 1024;
        mainFramebufferSpec.Resizable = true;
        mainFramebufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
                .TextureAttachment = Targets.MainDepthMap,
            },
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .DataFormat = TextureDataFormat::RGBA8,
                .Index = 0,
                .TextureAttachment = Targets.MainColorMap,
            }, // viewing result
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .DataFormat = TextureDataFormat::RedInteger,
                .Index = 1,
                .TextureAttachment = Targets.MainEntityIDMap,
            }, // entity id
        };

        ZPG_CORE_INFO("Creating main framebuffer");
        Targets.MainFrameBuffer = FrameBuffer::Create(mainFramebufferSpec);



        FrameBufferSpecification geometryFrameBufferSpec;
        geometryFrameBufferSpec.Width = 1024;
        geometryFrameBufferSpec.Height = 1024;
        geometryFrameBufferSpec.Resizable = true;
        geometryFrameBufferSpec.Attachments = {
            {
                .AttachType = FrameBufferAttachmentType::Depth,
                .DataFormat = TextureDataFormat::Depth32F,
                .Index = 0,
                .TextureAttachment = Targets.GeometryDepthMap,
            },
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .DataFormat = TextureDataFormat::RGBA32F,
                .Index = 0,
                .TextureAttachment = Targets.GeometryPositionMap,
            },
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .DataFormat = TextureDataFormat::RGBA32F,
                .Index = 1,
                .TextureAttachment = Targets.GeometryNormalMap,
            },
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .DataFormat = TextureDataFormat::RGBA32F,
                .Index = 2,
                .TextureAttachment = Targets.GeometryAlbedoMetallicMap,
            },
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .DataFormat = TextureDataFormat::RGBA32F,
                .Index = 3,
                .TextureAttachment = Targets.GeometryEmissiveRoughnessMap,
            },
            {
                .AttachType = FrameBufferAttachmentType::Color,
                .DataFormat = TextureDataFormat::RedInteger,
                .Index = 4,
                .TextureAttachment = Targets.GeometryEntityIDMap,
            },
        };

        ZPG_CORE_INFO("Creating geometry framebuffer");
        Targets.GeometryFrameBuffer = FrameBuffer::Create(geometryFrameBufferSpec);

    }

    RenderContext::~RenderContext()
    {
        ZPG_PROFILE_FUNCTION();
    }

    void RenderContext::DrawCommandQueues::Clear()
    {
        Shadow.clear();
        GeometryBuffer.clear();
        ForwardOpaque.clear();
        ForwardTransparent.clear();
    }

    void RenderContext::DrawBatches::Clear()
    {
        Shadow.clear();
        GeometryBuffer.clear();
        ForwardOpaque.clear();
        ForwardTransparent.clear();
    }

    void RenderContext::Lights::Clear()
    {
        PointLights.clear();
        SpotLights.clear();
        AmbientLight = nullptr;
        DirectionalLight = nullptr;
    }
}
