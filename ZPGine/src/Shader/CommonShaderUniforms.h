//
// Created by phatt on 10/17/25.
//

#ifndef COMMON_SHADER_UNIFORMS_H
#define COMMON_SHADER_UNIFORMS_H

#include "ShaderDataType.h"

namespace ZPG {


class CommonShaderUniforms {
    struct NameType {
        const std::string Handle;
        ShaderDataType::Type Type = ShaderDataType::Type::None;

        operator const char* () const { return Handle.c_str(); }
        operator const std::string& () const { return Handle; }
    };

    using T = ShaderDataType::Type;
public:

    inline static NameType VIEW {"u_View", T::Mat4};
    inline static NameType PROJ {"u_Proj", T::Mat4};
    inline static NameType MODEL {"u_Model", T::Mat4};
    inline static NameType VIEW_PROJ {"u_ViewProj", T::Mat4};

    inline static NameType CAMERA_POSITION {"u_CameraPos", T::Float3};

    // todo: Use uniform buffer objects.
    inline static NameType LIGHT_ARRAY {"u_Lights", T::None};
    inline static NameType LIGHT_COUNT {"u_LightCount", T::Int};
    inline static NameType AMBIENT_COLOR {"u_AmbientColor", T::Float4};

    inline static NameType ALBEDO {"u_Albedo", T::Float4};
    inline static NameType METALLIC {"u_Metallic", T::Float};
    inline static NameType ROUGHNESS {"u_Roughness", T::Float};
    inline static NameType EMISSIVE {"u_Emissive", T::Float};

    inline static NameType ALBEDO_MAP {"u_AlbedoMap", T::Int};
    inline static NameType NORMAL_MAP {"u_NormalMap", T::Int};
    inline static NameType METALNESS_MAP {"u_MetalnessMap", T::Int};
    inline static NameType ROUGHNESS_MAP {"u_RoughnessMap", T::Int};
    inline static NameType EMISSIVE_MAP {"u_EmissiveMap", T::Int};


    inline static NameType BLOCK_MATRICES {"ub_Matrices", T::Int};
    inline static NameType BLOCK_LIGHTS {"ub_Lights", T::Int};

    inline static NameType SKYTYPE {"u_SkyType", T::Int};
    inline static NameType SKYBOX_CUBEMAP {"u_SkyboxMap", T::Int};
    inline static NameType SKYDOME_MAP {"u_SkydomeMap", T::Int};


    inline static NameType DIRECTIONAL_LIGHT_SHADOW_MAP {"u_DirectionalLightShadowMap", T::Int};
    inline static NameType SPOTLIGHT_SHADOW_MAP_ARRAY {"u_SpotLightShadowMapArray", T::Int};
    inline static NameType POINTLIGHT_SHADOW_CUBE_MAP_ARRAY {"u_PointLightShadowCubeMapArray", T::Int};

    inline static NameType GEOMETRY_POSITION_MAP {"g_PositionMap", T::Int};
    inline static NameType GEOMETRY_NORMAL_MAP {"g_NormalMap", T::Int};
    inline static NameType GEOMETRY_ALBEDO_METALLIC_MAP {"g_AlbedoMetallicMap", T::Int};
    inline static NameType GEOMETRY_EMISSIVE_ROUGHNESS_MAP {"g_EmissiveRoughnessMap", T::Int};
    inline static NameType GEOMETRY_ENTITY_ID_MAP {"g_EntityIDMap", T::Int};

    // helper to map correct uniform names regarding the light array in fragment shaders
    class LightArray {
    public:
        inline static NameType LIGHT_TYPE {"Type", T::Int};
        inline static NameType LIGHT_COLOR {"Color", T::Float4};
        inline static NameType LIGHT_POSITION {"Pos", T::Float3};
        inline static NameType LIGHT_DIRECTION {"Dir", T::Float3};
        inline static NameType LIGHT_INNER_CUTOFF {"InCutoff", T::Float};
        inline static NameType LIGHT_OUTER_CUTOFF {"OutCutoff", T::Float};
        inline static NameType LIGHT_BEAM_SIZE {"BeamSize", T::Float};
        inline static NameType LIGHT_BEAM_BLEND {"BeamBlend", T::Float};

        static std::string Type(u32 index) { return Indexed(index) + "." + LIGHT_TYPE.Handle; }
        static std::string Color(u32 index) { return Indexed(index) + "." + LIGHT_COLOR.Handle; }
        static std::string Position(u32 index) { return Indexed(index) + "." + LIGHT_POSITION.Handle; }
        static std::string Direction(u32 index) { return Indexed(index) + "." + LIGHT_DIRECTION.Handle; }
        static std::string InnerCutoff(u32 index) { return Indexed(index) + "." + LIGHT_INNER_CUTOFF.Handle; }
        static std::string OuterCutoff(u32 index) { return Indexed(index) + "." + LIGHT_OUTER_CUTOFF.Handle; }
        static std::string BeamSize(u32 index) { return Indexed(index) + "." + LIGHT_BEAM_SIZE.Handle; }
        static std::string BeamBlend(u32 index) { return Indexed(index) + "." + LIGHT_BEAM_BLEND.Handle; }

        static std::string Indexed(u32 index) { return std::string(LIGHT_ARRAY.Handle) + "[" + std::to_string(index) + "]"; }
    };
};

/*
static void foo() {
    CommonShaderUniforms::ALBEDO;
    auto a = CommonShaderUniforms::LightArray::Type(3);
    auto b = CommonShaderUniforms::LightArray::Color(3);
}
*/

}

#endif
