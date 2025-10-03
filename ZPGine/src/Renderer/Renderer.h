//
// Created by phatt on 9/22/25.
//

#ifndef RENDERER_H
#define RENDERER_H

#include "Camera.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "RendererAPI.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include "Light/Light.h"

namespace ZPG {

// Only Renderer should use RenderCommand. 
class Renderer {
public:
    static void Init();  
    static void Shutdown();
    static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

    static void BeginDraw(const Camera& camera);
    static void EndDraw();

    static void SetLights(const std::vector<Ref<Light>>& lights);
    static void Submit(const Ref<ShaderProgram>& shaderProgram, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::identity<glm::mat4>());
    
    static void OnWindowResize(int width, int height);
private:
    struct DrawData {
        glm::mat4 ViewProjMatrix;
        std::vector<Ref<Light>> Lights;
    };
    inline static Scope<DrawData> s_DrawData = CreateScope<DrawData>();
};

}

#endif //RENDERER_H
