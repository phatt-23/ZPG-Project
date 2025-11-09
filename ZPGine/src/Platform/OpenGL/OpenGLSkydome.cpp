#include "OpenGLSkydome.h"

#include "Buffer/VertexArray.h"
#include "Profiling/Instrumentor.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture.h"

namespace ZPG 
{

OpenGLSkydome::OpenGLSkydome(const SkydomeSpecification& spec)
    : m_Spec(spec)
{
    ZPG_PROFILE_FUNCTION();
    m_VertexArray = ResourceManager::GetGlobal().GetMesh(CommonResources::MESH_SKYDOME)->GetVertexArray();
    m_ShaderProgram = ResourceManager::GetGlobal().GetShaderProgram(CommonResources::SHADER_PROGRAM_SKYDOME);

    m_Texture = Texture::Create(m_Spec.TexturePath);
}

OpenGLSkydome::~OpenGLSkydome()
{
}

const ref<VertexArray>& OpenGLSkydome::GetVertexArray() const
{
    ZPG_PROFILE_FUNCTION();
    return m_VertexArray;
}

void OpenGLSkydome::BindTextureToSlot(int slot) const
{
    ZPG_PROFILE_FUNCTION();
    m_Texture->BindToSlot(slot);
}

void OpenGLSkydome::Bind() const
{
    ZPG_PROFILE_FUNCTION();
    m_ShaderProgram->Bind();
    m_ShaderProgram->SetInt("u_SkydomeMap", 0);
    BindTextureToSlot(0);
    m_VertexArray->Bind();
}

void OpenGLSkydome::Unbind() const
{
    ZPG_PROFILE_FUNCTION();
    m_VertexArray->Unbind();
    m_ShaderProgram->Unbind();
}

}
