//
// Created by phatt on 10/16/25.
//

#ifndef COMMON_RESOURCES_H
#define COMMON_RESOURCES_H


namespace ZPG {

class CommonResources {
public:
    constexpr static auto NULL_MATERIAL = "mat://Material_Null";

    constexpr static auto NULL_ALBEDO_MAP = "tex://Albedo_Null";
    constexpr static auto NULL_NORMAL_MAP = "tex://Normal_Null";
    constexpr static auto NULL_METALNESS_MAP = "tex://Metalness_Null";
    constexpr static auto NULL_ROUGHNESS_MAP = "tex://Roughness_Null";
    constexpr static auto NULL_EMISSIVE_MAP = "tex://Emissive_Null";

    constexpr static auto SHADER_PROGRAM_DEFAULT = "shader://Default";

    constexpr static auto SHADER_PROGRAM_CONSTANT = "shader://Constant";
    constexpr static auto SHADER_PROGRAM_LAMBERT = "shader://Lambert";
    constexpr static auto SHADER_PROGRAM_PHONG = "shader://Phong";
    constexpr static auto SHADER_PROGRAM_BLINN_PHONG = "shader://Blinn-Phong";

    constexpr static auto SHADER_PROGRAM_PBR = "shader://PBR";

    constexpr static auto MODEL_SPHERE = "model://Sphere";
    constexpr static auto MODEL_BOX = "model://Box";

    constexpr static auto VAO_SPHERE = "vao://Sphere";
    constexpr static auto VAO_BOX = "vao://Box";

    constexpr static auto MESH_SPHERE = "mesh://Sphere";
    constexpr static auto MESH_BOX = "mesh://Box";
};

}

#endif //COMMON_RESOURCES_H
