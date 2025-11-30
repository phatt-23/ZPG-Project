#include "ForwardTransparentRenderPass.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Renderer/DrawCommand.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Renderer/RenderCommand.h"
#include "Shader/CommonShaderUniforms.h"
#include "Texture/Texture2DArray.h"

static const char* vertexShader = R"(
    #version 460 core

    layout(location = 0) in vec3 a_Pos;
    layout(location = 1) in vec3 a_Normal;
    layout(location = 2) in vec2 a_TexCoord;
    layout(location = 3) in vec3 a_Tangent;

    uniform mat4 u_Model;

    out vec3 v_WorldPos;
    out vec3 v_WorldNormal;
    out vec2 v_TexCoord;
    out mat3 v_TBN;

    #include "ext/ssbo/CameraSSBO.glsl"

    void main(void)
    {
        mat4 model = u_Model;
            
        vec4 worldPos = model * vec4(a_Pos, 1.0);
        mat3 normalMat = transpose(inverse(mat3(model)));

        v_WorldPos = worldPos.xyz / worldPos.w;
        v_WorldNormal = normalize(normalMat * a_Normal);
        v_TexCoord = a_TexCoord;

        vec3 N = v_WorldNormal;
        vec3 T = normalize(normalMat * a_Tangent);
        T = normalize(T - dot(T, N) * N);
        vec3 B = normalize(cross(N, T));

        v_TBN = mat3(T, B, N);

        gl_Position = ssbo_Camera.ViewProj * worldPos;
    }
)";

static const char* fragmentShader = R"(
    #version 460 core

    #include "ext/utils.glsl"
    #include "ext/ssbo/EnvironmentLightSSBO.glsl"
    #include "ext/ssbo/CameraSSBO.glsl"
    #include "ext/ssbo/MaterialSSBO.glsl"
    #include "ext/ssbo/SpotLightSSBO.glsl"
    #include "ext/ssbo/PointLightSSBO.glsl"
    #include "ext/light/LightingProps.glsl"
    #define CONVERSIONS_GLSL_IMPLEMENTATION
    #include "ext/conversions.glsl"
    #define CALC_LIGHT_GLSL_IMPLEMENTATION
    #include "ext/light/CalcLight.glsl"
    #define CALC_SHADOW_GLSL_IMPLEMENTATION
    #include "ext/shadow/CalcShadow.glsl"


    in vec3 v_WorldPos;
    in vec3 v_WorldNormal;
    in vec2 v_TexCoord;
    in mat3 v_TBN;

    uniform int u_EntityID;

    uniform sampler2DArray u_DirectionalLightShadowMapArray;

    layout(location = 0) out vec4 f_Color0;
    layout(location = 1) out int f_Color1;
    layout(location = 2) out vec4 f_Color2;

    void main(void)
    {
        PBRProps pbr;
        pbr.Albedo = texture(u_AlbedoMap, v_TexCoord).rgba;
        pbr.Metallic = texture(u_MetalnessMap, v_TexCoord).a;
        pbr.Roughness = texture(u_RoughnessMap, v_TexCoord).a;
        pbr.Emissive = texture(u_EmissiveMap, v_TexCoord).rgba;

        if(pbr.Albedo.a < 0.1) {
            discard;
        }

        PhongProps phong = ConvertPBRToPhong(pbr);
        phong.DiffuseColor = mix(phong.DiffuseColor, pbr.Albedo, 0.2);

        vec3 P = v_WorldPos;
        vec3 N = normalize(v_WorldNormal);
        vec3 V = normalize(ssbo_Camera.CameraPosition - P);

        vec3 Lo = vec3(0.0);
        vec3 Le = ScaleByA(phong.DiffuseColor * phong.EmissiveColor);
        vec3 La = ScaleByA(phong.DiffuseColor * ssbo_EnvironmentLight.AmbientColor);

        // directional light
        {
            DirectionalLight light = ssbo_EnvironmentLight.DirLight;
            float shadow = CalcShadowDirectional(u_DirectionalLightShadowMapArray, P, N, light, ssbo_Camera.View);
            Lo += CalcLightDirectional(light, phong, V, N, shadow);
        }

        // point lights
        for (int i = 0; i < ssbo_PointLight.Count; i++)
        {
            PointLight light = ssbo_PointLight.LightArray[i];
            float shadow = 0.0;
            Lo += CalcLightPoint(light, phong, P, V, N, shadow);
        }

        // spotlights
        for (int i = 0; i < ssbo_SpotLight.Count; i++)
        {
            SpotLight light = ssbo_SpotLight.LightArray[i];
            float shadow = 0.0;
            Lo += CalcLightSpot(light, phong, P, V, N, shadow);
        }

        vec3 color = Lo + La + Le;

        float brightness = dot(color, vec3(0.7, 0.7, 0.7));

        f_Color0 = vec4(color, pbr.Albedo.a);
        f_Color1 = u_EntityID;
        f_Color2 = vec4((brightness > 1.0) ? color : vec3(0.0), 1.0);
    }
)";

namespace ZPG
{
    void ForwardTransparentRenderPass::Init(RenderContext& context)
    {
        m_FrameBuffer = context.Targets.MainFrameBuffer;

        m_ShaderProgram = ShaderProgram::Create("forward_transparent.program", 
        {
            Shader::CreateFromCode("forward_transparent.vert", Shader::Vertex, vertexShader),
            Shader::CreateFromCode("forward_transparent.frag", Shader::Fragment, fragmentShader),
        });
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP, RenderBindingPoints::ALBEDO_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP, RenderBindingPoints::NORMAL_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, RenderBindingPoints::ROUGHNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP, RenderBindingPoints::METALNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::EMISSIVE_MAP, RenderBindingPoints::EMISSIVE_MAP);
        m_ShaderProgram->Unbind();
    }

    void ForwardTransparentRenderPass::Execute(RenderContext& context)
    {
        m_FrameBuffer->Bind(); 
        m_ShaderProgram->Bind();

        m_ShaderProgram->SetInt(CommonShaderUniforms::DIRECTIONAL_LIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP_ARRAY);
        context.Targets.DirectionalLightShadowMapArray->BindToSlot(RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP_ARRAY);

        // glEnable(GL_BLEND);
        // glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (const auto& cmd : std::views::concat(
            context.Queues.ForwardTransparent,
            context.StaticQueues.ForwardTransparent)
        )
        {
            Material& material = *cmd.mesh->GetMaterial();

            m_ShaderProgram->SetInt("u_EntityID", cmd.entityID);
            m_ShaderProgram->SetMat4("u_Model", cmd.transform);
        
            context.SSBO.MaterialSSBO.SetMaterial(material);
            material.BindMaps();

            RenderCommand::DrawInstanced(*cmd.mesh->GetVertexArray(), 1);
        }

        glDisable(GL_BLEND);
        // glDepthMask(GL_TRUE);

        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind(); 
    }

}
