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

    static void Submit(const Ref<ShaderProgram>& shaderProgram, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::identity<glm::mat4>());
    static void Submit(const Ref<ShaderProgram>& shaderProgram, const Ref<Mesh>& mesh, const glm::mat4& transform = glm::mat4(1.0f));
    static void Submit(const Ref<ShaderProgram>& shaderProgram, const Ref<Entity>& entity, const glm::mat4& transform = glm::mat4(1.0f));

    static void OnWindowResize(int width, int height);

    static void LoadShaderProgram(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    static Ref<ShaderProgram> GetShaderProgram(const std::string& name);
private:
    struct DrawData {
        glm::mat4 ViewProjMatrix;
        std::vector<Ref<Light>> Lights;
        glm::vec3 CameraPosition;
    };
    inline static Scope<DrawData> s_DrawData = nullptr; 
    inline static Scope<ShaderProgramLibrary> s_ShaderProgramLibrary = nullptr;
};

}

#endif //RENDERER_H
