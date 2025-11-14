#pragma once

namespace ZPG
{
    namespace RenderBindingPoints
    {
        /**
         * Texture unit slots
         */
        constexpr int ALBEDO_MAP = 0;
        constexpr int METALNESS_MAP = 1;
        constexpr int ROUGHNESS_MAP = 2;
        constexpr int NORMAL_MAP = 3;
        constexpr int EMISSIVE_MAP = 4;

        constexpr int SKYBOX_TEXTURE_SLOT = 10;
        constexpr int SKYDOME_TEXTURE_SLOT = 11;

        constexpr int DIRECTIONAL_LIGHT_SHADOW_MAP = 12;
        constexpr int SPOTLIGHT_SHADOW_MAP_ARRAY = 13;
        constexpr int POINTLIGHT_SHADOW_CUBE_MAP_ARRAY = 14;


        /**
         * Shader storage buffer objects.
         */
        constexpr int CAMERA_SSBO = 0;
        constexpr int ENVIRONMENT_LIGHT_SSBO = 1;
        constexpr int POINT_LIGHT_SSBO = 2;
        constexpr int SPOT_LIGHT_SSBO = 3;
        constexpr int MATERIAL_SSBO = 4;
        constexpr int MODELS_SSBO = 5;
        constexpr int ENTITY_SSBO = 6;
    };

}
