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
        ShaderDataType::Type Type;

        operator const char* () const { return Handle.c_str(); }
        operator const std::string& () const { return Handle; }
    };

    using T = ShaderDataType::Type;
public:

    constexpr static NameType VIEW {"u_View", T::Mat4};
    constexpr static NameType PROJ {"u_Proj", T::Mat4};
    constexpr static NameType MODEL {"u_Model", T::Mat4};
    constexpr static NameType VIEW_PROJ {"u_ViewProj", T::Mat4};

    constexpr static NameType CAMERA_POSITION {"u_CameraPos", T::Float3};

    // todo: Use uniform buffer objects.
    constexpr static NameType LIGHT_ARRAY {"u_Lights", T::None};
    constexpr static NameType LIGHT_COUNT {"u_LightCount", T::Int};
    constexpr static NameType AMBIENT_COLOR {"u_AmbientColor", T::Float4};

    constexpr static NameType ALBEDO {"u_Albedo", T::Float4};
    constexpr static NameType METALLIC {"u_Metallic", T::Float};
    constexpr static NameType ROUGHNESS {"u_Roughness", T::Float};
    constexpr static NameType EMISSIVE {"u_Emissive", T::Float};

    constexpr static NameType ALBEDO_MAP {"u_AlbedoMap", T::Int};
    constexpr static NameType NORMAL_MAP {"u_NormalMap", T::Int};
    constexpr static NameType METALNESS_MAP {"u_MetalnessMap", T::Int};
    constexpr static NameType ROUGHNESS_MAP {"u_RoughnessMap", T::Int};

    constexpr static NameType BLOCK_MATRICES {"ub_Matrices", T::Int};
    constexpr static NameType BLOCK_LIGHTS {"ub_Lights", T::Int};


    // helper to map correct uniform names regarding the light array in fragment shaders
    class LightArray {
    public:
        constexpr static NameType LIGHT_TYPE {"Type", T::Int};
        constexpr static NameType LIGHT_COLOR {"Color", T::Float4};
        constexpr static NameType LIGHT_POSITION {"Pos", T::Float3};
        constexpr static NameType LIGHT_DIRECTION {"Dir", T::Float3};
        constexpr static NameType LIGHT_INNER_CUTOFF {"InCutoff", T::Float};
        constexpr static NameType LIGHT_OUTER_CUTOFF {"OutCutoff", T::Float};
        constexpr static NameType LIGHT_BEAM_SIZE {"BeamSize", T::Float};
        constexpr static NameType LIGHT_BEAM_BLEND {"BeamBlend", T::Float};

        static std::string Type(u32 index) { return Indexed(index) + "." + LIGHT_TYPE.Handle; }
        static std::string Color(u32 index) { return Indexed(index) + "." + LIGHT_COLOR.Handle; }
        static std::string Position(u32 index) { return Indexed(index) + "." + LIGHT_POSITION.Handle; }
        static std::string Direction(u32 index) { return Indexed(index) + "." + LIGHT_DIRECTION.Handle; }
        static std::string InnerCutoff(u32 index) { return Indexed(index) + "." + LIGHT_INNER_CUTOFF.Handle; }
        static std::string OuterCutoff(u32 index) { return Indexed(index) + "." + LIGHT_OUTER_CUTOFF.Handle; }
        static std::string BeamSize(u32 index) { return Indexed(index) + "." + LIGHT_BEAM_SIZE.Handle; }
        static std::string BeamBlend(u32 index) { return Indexed(index) + "." + LIGHT_BEAM_BLEND.Handle; }

        static std::string Indexed(u32 index) { return LIGHT_ARRAY.Handle + "[" + std::to_string(index) + "]"; }
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
