//
// Created by phatt on 9/22/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "RendererAPI.h"

namespace ZPG {

class Model;
class ShaderProgram;
class Mesh;
class Entity;
class Scene;
class Light;
class Camera;

// Only Renderer should use RenderCommand. 
// It owns the shader program library. Fits best in here. 
// Without a shader program, there is no point in rendering any object.
class Renderer {
public:
    static void Init();  // must be called explicitly during Application initialization
    static void Shutdown();
    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    static void BeginDraw(const Camera& camera);
    static void EndDraw();

    static void SetLights(const std::vector<ref<Light>>& lights);

    // Render a whole scene. Pulls the drawable objects from the scene, the lights and camera.
    // Can optimize by grouping the objects by group-by('shaderProgram', 'material').
    // Entities that are instances of the same model are drawn by instantiated draw.
    // Objects that share shaderPrograms are drawn together to reduce the comm between
    // CPU and GPU.
    // Queries the scene, groups the drawable objects and batch draws them.
    static void RenderScene(const Scene& scene);

    // Methods below allow the client to submit individual geometries with shader programs and transforms. 
    
    // new API
    // Draw the entity and also draws it into the stencil buffer with color specified by its ID.
    static void SubmitEntity(const Entity& entity, const m4& transform = m4(1.0f));
    static void SubmitMesh(const Mesh& mesh, const m4& transform = m4(1.0f));

    // old API
    // ShaderProgram, VAO, Transform
    static void Submit(ShaderProgram& shaderProgram, const VertexArray& vertexArray, const m4& transform = m4(1.0f));
    // ShaderProgram, Mesh, Transform
    static void Submit(ShaderProgram& shaderProgram, const Mesh& mesh, const m4& transform = m4(1.0f));
    // ShaderProgram, Model, Transform
    static void Submit(ShaderProgram& shaderProgram, const Model& model, const m4& transform = m4(1.0f));
    // ShaderProgram, Entity, Transform
    // this method doesn't make sense anymore
    static void Submit(ShaderProgram& shaderProgram, const Entity& entity, const m4& transform = m4(1.0f));

    static void OnWindowResize(int width, int height);
private:
    struct DrawData {
        m4 ViewProjMatrix;
        std::vector<ref<Light>> Lights;
        v3 CameraPosition;
    };
    inline static scope<DrawData> s_DrawData = nullptr; 
};

}

#endif //RENDERER_H
