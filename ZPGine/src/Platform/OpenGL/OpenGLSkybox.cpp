//
// Created by phatt on 11/1/25.
//

#include "OpenGLSkybox.h"

#include "OpenGLCore.h"
#include "Buffer/BufferLayout.h"
#include "Buffer/IndexBuffer.h"
#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Debug/Asserter.h"
#include "Shader/Shader.h"
#include "Shader/ShaderDataType.h"
#include "Shader/ShaderProgram.h"
#include "stb_image/stb_image.h"

namespace ZPG {

constexpr static f32 skyboxVertices[] = {
    // position
    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
};

constexpr static u32 skyboxIndices[] = {
    0, 1, 2,   0, 2, 3, // Front face
    0, 4, 7,   0, 7, 3, // Left face
    3, 7, 6,   3, 6, 2, // Right face
    2, 6, 5,   2, 5, 1, // Back face
    1, 5, 4,   1, 4, 0, // Bottom face
    4, 5, 6,   4, 6, 7, // Top face
};

const char* skyboxVertexSource = R"(
    #version 440 core

    layout(location = 0) in vec3 a_Pos;

    layout(std430, binding = 0) buffer MatricesStorageBuffer {
        mat4 View;
        mat4 Proj;
        mat4 ViewProj;
    } ssbo_Matrices;

    out vec3 v_TexCoords;

    void main() {
        v_TexCoords = a_Pos;
        mat4 viewNoTrans = mat4(mat3(ssbo_Matrices.View));
        vec4 pos = ssbo_Matrices.Proj * viewNoTrans * vec4(a_Pos, 1.0);
        gl_Position = pos.xyww;
    })";

const char* skyboxFragmentSource = R"(
    #version 440 core

    in vec3 v_TexCoords;
    uniform samplerCube u_SkyboxMap;
    out vec4 f_FragColor;

    void main() {
        vec3 col = texture(u_SkyboxMap, v_TexCoords).rgb;
        f_FragColor = vec4(col, 1.0); // ensure alpha or packed metalness as needed
    })";


OpenGLSkybox::OpenGLSkybox(const SkyboxSpecification& spec)
    : m_Specification(spec)
{
    // Cube Map texture.
    ZPG_OPENGL_CALL(glGenTextures(1, &m_TextureRendererID));
    ZPG_OPENGL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureRendererID));

    stbi_set_flip_vertically_on_load(false);

    std::filesystem::path directory(m_Specification.Directory + (m_Specification.Directory.ends_with('/') ? "" : "/"));

    for (u32 i = 0; i < m_Specification.Filenames.size(); i++) {
        ZPG_CORE_INFO("Loading cube-map textures - directory: %s, file: %s\n", directory.c_str(), m_Specification.Filenames[i].c_str());

        int width, height, nrChannels;
        unsigned char* data = stbi_load((directory / m_Specification.Filenames[i]).c_str(), &width, &height, &nrChannels, 0);

        ZPG_CORE_ASSERT(data, "Cube map texture failed to load: {}", directory.c_str());

        ZPG_OPENGL_CALL(glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height,
            0, GL_RGB, GL_UNSIGNED_BYTE, data));

        stbi_image_free(data);
    }

    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    ZPG_OPENGL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

    // Shader program
    {
        m_ShaderProgram = ShaderProgram::Create("SkyboxSP", {
            Shader::CreateFromCode("SkyboxSP.vert", Shader::Vertex, skyboxVertexSource),
            Shader::CreateFromCode("SkyboxSP.frag", Shader::Fragment, skyboxFragmentSource),
        });

        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt("u_SkyboxMap", 0);
        m_ShaderProgram->Unbind();
    }

    // Skybox VAO
    {
        ref<VertexBuffer> skyboxVBO = VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices), { {ShaderDataType::Float3, "a_Pos"} });

        ref<IndexBuffer> skyboxIBO = IndexBuffer::Create(skyboxIndices, sizeof(skyboxIndices));

        m_VAO = VertexArray::Create({ skyboxVBO }, skyboxIBO);
    }

}

OpenGLSkybox::~OpenGLSkybox() {
    glDeleteTextures(1, &m_TextureRendererID);
}

void OpenGLSkybox::Bind() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureRendererID);
    m_ShaderProgram->Bind();
    m_VAO->Bind();
}

void OpenGLSkybox::Unbind() {
    m_VAO->Unbind();
    m_ShaderProgram->Unbind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

const ref<VertexArray>& OpenGLSkybox::GetVAO() const {
    return m_VAO;
}

} // ZPG