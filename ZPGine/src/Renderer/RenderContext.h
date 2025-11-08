#pragma once

#include "RenderStatistics.h"
#include "Buffer/FrameBuffer.h"
#include "Renderer/RenderBatch.h"
#include "Camera/Camera.h"
#include "Sky/Sky.h"
#include "LightStruct/LightStructs.h"
#include "Renderer/ShaderStorageBuffer/CameraShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/EntityShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/EnvironmentLightShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/PointLightShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/SpotLightShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/ModelShaderStorageBuffer.h"
#include "Renderer/ShaderStorageBuffer/MaterialShaderStorageBuffer.h"
#include "Entity/Entity.h"

namespace ZPG
{
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
        /**
         * Provides view, projection matrices and camera position.
         */
        Camera* ActiveCamera = nullptr;


        /**
         * Could be skybox ar skydome
         */
        ref<Sky> ActiveSky = nullptr;
       
        /**
         * Visible entities
         */
        std::vector<ref<Entity>> VisibleEntities;

        /** 
        * Lights represented in the same form as in shader programs.
        */
        std::vector<PointLightStruct> PointLights;
        std::vector<SpotLightStruct> SpotLights;
        AmbientLightStruct* AmbientLight = nullptr;
        DirectionalLightStruct* DirectionalLight = nullptr;

        
        /**
         * Shader storage buffers.
         */
        CameraShaderStorageBuffer CameraSSBO;

        EnvironmentLightShaderStorageBuffer EnvironmentLightSSBO;
        PointLightShaderStorageBuffer PointLightSSBO;
        SpotLightShaderStorageBuffer SpotLightSSBO;

        MaterialShaderStorageBuffer MaterialSSBO;
        ModelShaderStorageBuffer ModelSSBO;
        EntityShaderStorageBuffer EntitySSBO;


        /**
         * Shadow Pass 
         */

        u32 DirectionalShadowMap;
        u32 SpotLightShadowMapArray;
        u32 PointLightShadowMapArray;


        ref<FrameBuffer> DirectionalLightShadowFramebuffer;
        ref<FrameBuffer> PointLightShadowFramebuffer;
        ref<FrameBuffer> SpotLightShadowFramebuffer;
        ref<FrameBuffer> GeometryPassFramebuffer;
        ref<FrameBuffer> MainFramebuffer;

    

        /**
        * Batch
        */
        RenderBatch Batch;

        /**
         * Statistics
         */
        RenderStatistics Statistics;
    };
}
