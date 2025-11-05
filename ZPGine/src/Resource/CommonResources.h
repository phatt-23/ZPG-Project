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
    constexpr static auto NULL_CUBEMAP = "tex://Cubemap_Null";
    constexpr static auto NULL_SKYDOME_MAP = "tex://Skybox_Null";

    constexpr static auto SHADER_PROGRAM_DEFAULT = "shader://Default";
    constexpr static auto SHADER_PROGRAM_CONSTANT = "shader://Constant";
    constexpr static auto SHADER_PROGRAM_LAMBERT = "shader://Lambert";
    constexpr static auto SHADER_PROGRAM_PHONG = "shader://Phong";
    constexpr static auto SHADER_PROGRAM_BLINN_PHONG = "shader://Blinn-Phong";
    constexpr static auto SHADER_PROGRAM_PBR = "shader://PBR";
    constexpr static auto SHADER_PROGRAM_SKYBOX = "shader://Skybox";
    constexpr static auto SHADER_PROGRAM_SKYDOME = "shader://Skydome";

    constexpr static auto MODEL_SPHERE = "model://Sphere";
    constexpr static auto MODEL_BOX = "model://Box";
    constexpr static auto MODEL_SKYDOME = "model://Skydome";
    constexpr static auto MODEL_SKYBOX = "model://Skybox";

    constexpr static auto VAO_SPHERE = "vao://Sphere";
    constexpr static auto VAO_BOX = "vao://Box";
    constexpr static auto VAO_SKYDOME = "vao://Skydome";
    constexpr static auto VAO_SKYBOX = "vao://Skybox";

    constexpr static auto MESH_SPHERE = "mesh://Sphere";
    constexpr static auto MESH_BOX = "mesh://Box";
    constexpr static auto MESH_SKYBOX = "mesh://Skybox";
    constexpr static auto MESH_SKYDOME = "mesh://Skydome";

};

}

#endif //COMMON_RESOURCES_H
