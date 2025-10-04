#pragma once

#include <glm/glm.hpp>
#include "Core/Core.h"

namespace ZPG {

// helper to map correct uniform names regarding the light array in fragment shaders
class LightArrayUniformNamer {
public:
    static std::string TypeUniName(u32 index) { return GetIndexedArray(index) + "." + "type"; }
    static std::string ColorUniName(u32 index) { return GetIndexedArray(index) + "." + "color"; }
    static std::string PositionUniName(u32 index) { return GetIndexedArray(index) + "." + "pos"; }
    static std::string DirectionUniName(u32 index) { return GetIndexedArray(index) + "." + "dir"; }
    static std::string InnerCutoffUniName(u32 index) { return GetIndexedArray(index) + "." + "inCutoff"; }
    static std::string OuterCutoffUniName(u32 index) { return GetIndexedArray(index) + "." + "outCutoff"; }
private:
    static std::string GetIndexedArray(u32 index) { return m_LightsArrayUniformName + "[" + std::to_string(index) + "]"; }
private:
    inline static const std::string m_LightsArrayUniformName = "u_Lights";
};

// struct LightStruct {
//     i32 Type;
//     glm::vec4 Color;
//     glm::vec3 Pos;
//     glm::vec3 Dir;
//     f32 InCutoff;
//     f32 OutCutoff;
// };

}
