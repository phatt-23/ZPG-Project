#pragma once

namespace ZPG
{
    namespace RenderBindingPoints
    {
        /**
         * Texture unit slots
         */

        constexpr int GEOMETRY_POSITION_MAP             = 0;
        constexpr int GEOMETRY_NORMAL_MAP               = 1;
        constexpr int GEOMETRY_ALBEDO_METALLIC_MAP      = 2;
        constexpr int GEOMETRY_EMISSIVE_ROUGHNESS_MAP  = 3;
        constexpr int GEOMETRY_ENTITY_ID_MAP            = 4;

        constexpr int ALBEDO_MAP    = 5;
        constexpr int METALNESS_MAP = 6;
        constexpr int ROUGHNESS_MAP = 7;
        constexpr int NORMAL_MAP    = 8;
        constexpr int EMISSIVE_MAP  = 9;

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
        constexpr int POINTLIGHT_SSBO = 2;
        constexpr int SPOTLIGHT_SSBO = 3;
        constexpr int MATERIAL_SSBO = 4;
        constexpr int MODEL_SSBO = 5;
        constexpr int ENTITY_SSBO = 6;
    };

}
