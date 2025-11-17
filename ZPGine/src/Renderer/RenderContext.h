#pragma once

#include "RenderStatistics.h"
#include "Buffer/FrameBuffer.h"
#include "Camera/Camera.h"
#include "Renderer/DrawBatch.h"
#include "Sky/Sky.h"
#include "LightStruct/LightStructs.h"
#include "Renderer/ShaderStorageBuffer/CameraShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/EntityShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/EnvironmentLightShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/PointLightShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/SpotLightShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/ModelShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/MaterialShaderStorageBuffer.h"

namespace ZPG
{
    class TextureCubeMapArray;
    class Texture2DArray;
    class DrawCommand;
    class DrawBatch;

    struct RenderContextSpecification
    {
        u32 PointLightCapacity;     
        u32 SpotLightCapacity;
        u32 BatchSize;
    };

    class RenderContext
    {
    public:
        RenderContext(const RenderContextSpecification& spec);
        ~RenderContext();

    public:
        Camera* ActiveCamera = nullptr;     // Provides view, projection matrices and camera position.
        Sky* ActiveSky = nullptr;           // Could be skybox ar skydome
       
        // Shader storage buffers.
        struct ShaderStorageBuffers 
        {
            CameraShaderStorageBuffer CameraSSBO;
            ModelShaderStorageBuffer ModelSSBO;
            EntityShaderStorageBuffer EntitySSBO;
            EnvironmentLightShaderStorageBuffer EnvironmentLightSSBO;
            PointLightShaderStorageBuffer PointLightSSBO;
            SpotLightShaderStorageBuffer SpotLightSSBO;
            MaterialShaderStorageBuffer MaterialSSBO;
        } SSBO;

        struct RenderTargets
        {
            // main attachment
            ref<FrameBuffer> MainFrameBuffer = nullptr;
            ref<Texture2D> MainDepthMap = nullptr;
            ref<Texture2D> MainColorMap = nullptr;
            ref<Texture2D> MainEntityIDMap = nullptr;

            // shadow map attachments
            ref<Texture2DArray> SpotLightShadowMapArray = nullptr;
            ref<TextureCubeMapArray> PointLightShadowCubeMapArray = nullptr;
            ref<Texture2D> DirectionalLightShadowMap = nullptr;

            // geometry buffer attachments
            ref<FrameBuffer> GeometryFrameBuffer = nullptr;
            ref<Texture2D> GeometryDepthMap = nullptr;
            ref<Texture2D> GeometryPositionMap = nullptr;
            ref<Texture2D> GeometryNormalMap = nullptr;
            ref<Texture2D> GeometryAlbedoMetallicMap = nullptr;
            ref<Texture2D> GeometryEmissiveRoughnessMap = nullptr;
            ref<Texture2D> GeometryEntityIDMap = nullptr;

        } Targets;

        struct DrawCommandQueues
        {
            vec<DrawCommand> Shadow;
            vec<DrawCommand> GeometryBuffer;
            vec<DrawCommand> ForwardOpaque;
            vec<DrawCommand> ForwardTransparent;

            void Clear();
        } Queues, StaticQueues;

        struct DrawBatches
        {
            umap<DrawBatchKey, DrawBatch> Shadow;
            umap<DrawBatchKey, DrawBatch> GeometryBuffer;
            umap<DrawBatchKey, DrawBatch> ForwardOpaque;
            umap<DrawBatchKey, DrawBatch> ForwardTransparent;

            void Clear();
        } Batches, StaticBatches;

        struct Lights // Lights represented in the same form as in shader programs.
        {
            std::vector<PointLightStruct> PointLights;
            std::vector<SpotLightStruct> SpotLights;
            AmbientLightStruct* AmbientLight = nullptr;
            DirectionalLightStruct* DirectionalLight = nullptr;

            void Clear();
        } Lights;

        RenderStatistics Statistics;
    };
}
