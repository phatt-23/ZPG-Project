//
// Created by phatt on 9/22/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "Shader/ShaderProgramLibrary.h"
#include "VertexArray.h"
#include "Shader/ShaderProgram.h"
#include "RendererAPI.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Light/Light.h"
#include "Model/Mesh.h"
#include "Entity/Entity.h"
#include "Core/Scene.h"

namespace ZPG {

// Only Renderer should use RenderCommand. 
// It owns the shader program library. Fits best in here. 
// Without a shader program, there is no point in rendering any object.
class Renderer {
public:
    static void Init();  // must be called explicitely during Application initialization
    static void Shutdown();
    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    static void BeginDraw(const Camera& camera);
    static void EndDraw();

    static void SetLights(const std::vector<Ref<Light>>& lights);

    // Render a whole scene. Pulls the drawable objects from the scene, the lights and camera.
    // Can optimize by grouping the objects by groupby('shaderProgram', 'material').
    // Entities that are instances of the same model are drawn by intantiated draw.
    // Objects that share shadrePrograms are drawn together to reduce the comm between 
    // CPU and GPU.
    // Queries the scene, groups the drawable objects and batch draws them.
    static void RenderScene(const Scene& scene);

    // Methods below allow the client to submit individual geometries with shader programs and transforms. 
    
    // new API
    // Draw the entity and also draws it into the stencil buffer with color specified by its ID.
    static void SumbitEntity(const Entity& entity, const glm::mat4& transform = glm::mat4(1.0f));
    static void SubmitMesh(const Mesh& mesh, const glm::mat4& transform = glm::mat4(1.0f));

    // old API
    // ShaderProgram, VAO, Transform
    static void Submit(ShaderProgram& shaderProgram, const VertexArray& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
    // ShaderProgram, Mesh, Transform
    static void Submit(ShaderProgram& shaderProgram, const Mesh& mesh, const glm::mat4& transform = glm::mat4(1.0f));
    // ShaderProgram, Model, Transform
    static void Submit(ShaderProgram& shaderProgram, const Model& model, const glm::mat4& transform = glm::mat4(1.0f));
    // ShaderProgram, Entity, Transform
    // this method doesn't make sense anymore
    static void Submit(ShaderProgram& shaderProgram, const Entity& entity, const glm::mat4& transform = glm::mat4(1.0f));

    static void OnWindowResize(int width, int height);
private:
    struct DrawData {
        glm::mat4 ViewProjMatrix;
        std::vector<Ref<Light>> Lights;
        glm::vec3 CameraPosition;
    };
    inline static Scope<DrawData> s_DrawData = nullptr; 
};

}

#endif //RENDERER_H
