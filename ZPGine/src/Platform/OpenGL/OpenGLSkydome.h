#pragma once
#include "Sky/Skydome.h"

namespace ZPG 
{
    class Mesh;
    class ShaderProgram;
    class Model;
    class Texture2D;
    class VertexArray;

    class OpenGLSkydome : public Skydome
    {
    public:
        OpenGLSkydome(const SkydomeSpecification& spec);
        virtual ~OpenGLSkydome() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual const ref<VertexArray>& GetVertexArray() const override;
        void BindTextureToSlot(int slot) const override;

    private:
        SkydomeSpecification m_Spec;
        ref<Texture2D> m_Texture;
        ref<ShaderProgram> m_ShaderProgram;
        ref<VertexArray> m_VertexArray;
    };
}
