//
// Created by phatt on 11/18/25.
//

#include "LightVolumeDeferredLightingBlinnPhongRenderPass.h"

#include "Renderer/LightStruct/LightStructs.h"
#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Model/ModelLoader.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Shader/ShaderDataType.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"
#include "Transform/TransformGroup.h"
#include "Transform/StaticTransform/Scale.h"
#include "Transform/StaticTransform/Rotate.h"
#include "Transform/StaticTransform/Translate.h"

namespace ZPG
{
class Rotate;

void LightVolumeDeferredLightingBlinnPhongRenderPass::Init(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        m_ShaderProgram = nullptr;

        m_EnvironmentLightShaderProgram = ShaderProgram::Create("environment_light.program",
        {
            Shader::CreateFromCode("environment_light.vert", Shader::Vertex, R"(
                #version 440 core

                layout(location = 0) in vec3 a_Pos;
                layout(location = 1) in vec2 a_TexCoord;

                out vec2 v_TexCoord;

                void main()
                {
                    v_TexCoord = a_TexCoord;
                    gl_Position = vec4(a_Pos, 1.0);
                }
            )"),
            Shader::CreateFromCode("environment_light.frag", Shader::Fragment, R"(
                #version 440 core

                // includes have to be in this order, i don't know why
                #include "ext/ssbo/EnvironmentLightSSBO.glsl"

                #include "ext/ssbo/CameraSSBO.glsl"

                #define CALC_SHADOW_GLSL_IMPLEMENTATION
                #include "ext/shadow/CalcShadow.glsl"

                layout(location = 0) out vec4 f_Color0;

                in vec2 v_TexCoord;

                uniform sampler2D g_PositionMap;
                uniform sampler2D g_NormalMap;
                uniform sampler2D g_AlbedoMetallicMap;
                uniform sampler2D g_EmissiveRoughnessMap;

                uniform sampler2D u_DirectionalLightShadowMap;

                void main()
                {
                    vec3 worldPos = texture(g_PositionMap, v_TexCoord).rgb;
                    vec3 N = normalize(texture(g_NormalMap, v_TexCoord).rgb * 2.0 - 1.0);
                    vec3 albedo = texture(g_AlbedoMetallicMap, v_TexCoord).rgb;
                    float metallic = texture(g_AlbedoMetallicMap, v_TexCoord).a;
                    vec3 emissive = texture(g_EmissiveRoughnessMap, v_TexCoord).rgb;
                    float roughness = texture(g_EmissiveRoughnessMap, v_TexCoord).a;

                    vec3 diffuseColor = albedo * clamp(1.0 - metallic, 0.01, 1.0);
                    float shininess = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);
                    vec3 baseSpecColor = mix(vec3(0.04), albedo, metallic);
                    float specIntensity = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
                    vec3 specularColor = baseSpecColor * specIntensity;

                    vec3 ambient = ssbo_EnvironmentLight.AmbientColor.rgb * ssbo_EnvironmentLight.AmbientColor.a;
                    vec3 V = normalize(ssbo_Camera.CameraPosition - worldPos);

                    // directional light
                    DirectionalLight light = ssbo_EnvironmentLight.DirLight;

                    float shadow = 1.0 - CalcShadowDirectional(u_DirectionalLightShadowMap, worldPos, N, light);

                    vec3 L = normalize(-light.Direction);
                    vec3 H = normalize(L + V);

                    float NdotL = max(dot(N, L), 0.0);
                    float NdotH = max(dot(N, H), 0.0);

                    vec3 diffuse = NdotL * diffuseColor;
                    vec3 specular = pow(NdotH, shininess) * specularColor;

                    vec3 La = albedo * ambient;
                    vec3 Lo = (diffuse + specular) * (light.Color.rgb * light.Color.a) * shadow;
                    vec3 Le = albedo * emissive;

                    vec3 color = Lo + Le + La;
                    color = color / (color + vec3(1.0));
                    color = pow(color, vec3(1.0 / 2.2));
                    f_Color0 = vec4(color, 1.0);
                }
            )"),
        });
        m_EnvironmentLightShaderProgram->Bind();
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_POSITION_MAP, RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_NORMAL_MAP, RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ALBEDO_METALLIC_MAP, RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_EMISSIVE_ROUGHNESS_MAP, RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::DIRECTIONAL_LIGHT_SHADOW_MAP, RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);
        m_EnvironmentLightShaderProgram->Unbind();

        m_PointLightShaderProgram = ShaderProgram::Create("point_light.program",
        {
            Shader::CreateFromCode("point_light.vert", Shader::Vertex, R"(
                #version 440 core

                #include "ext/ssbo/CameraSSBO.glsl"

                layout(location = 0) in vec3 a_Pos;
                layout(location = 1) in vec3 a_Normal;
                layout(location = 2) in vec2 a_TexCoord;
                layout(location = 3) in vec3 a_Tangent;

                uniform mat4 u_Model;

                void main()
                {
                    gl_Position = ssbo_Camera.ViewProj * u_Model * vec4(a_Pos, 1.0);
                }
            )"),
            Shader::CreateFromCode("point_light.frag", Shader::Fragment, R"(
                #version 440 core

                #include "ext/ssbo/CameraSSBO.glsl"
                #include "ext/ssbo/PointLightSSBO.glsl"

                #define CALC_SHADOW_GLSL_IMPLEMENTATION
                #include "ext/shadow/CalcShadow.glsl"

                layout(location = 0) out vec4 f_Color0;

                uniform int u_Index;
                uniform vec2 u_ScreenSizeInv;

                uniform sampler2D g_PositionMap;
                uniform sampler2D g_NormalMap;
                uniform sampler2D g_AlbedoMetallicMap;
                uniform sampler2D g_EmissiveRoughnessMap;

                void main()
                {
                    vec2 texCoord = gl_FragCoord.xy * u_ScreenSizeInv;

                    vec3 worldPos = texture(g_PositionMap, texCoord).rgb;
                    vec3 N = normalize(texture(g_NormalMap, texCoord).rgb * 2.0 - 1.0);
                    vec3 albedo = texture(g_AlbedoMetallicMap, texCoord).rgb;
                    float metallic = texture(g_AlbedoMetallicMap, texCoord).a;
                    vec3 emissive = texture(g_EmissiveRoughnessMap, texCoord).rgb;
                    float roughness = texture(g_EmissiveRoughnessMap, texCoord).a;

                    vec3 diffuseColor = albedo * clamp(1.0 - metallic, 0.01, 1.0);
                    float shininess = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);
                    vec3 baseSpecColor = mix(vec3(0.04), albedo, metallic);
                    float specIntensity = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
                    vec3 specularColor = baseSpecColor * specIntensity;

                    PointLight light = ssbo_PointLight.LightArray[u_Index];

                    vec3 L = normalize(light.Position - worldPos);
                    vec3 V = normalize(ssbo_Camera.CameraPosition - worldPos);
                    vec3 H = normalize(V + L);

                    float dist = length(light.Position - worldPos);
                    float atten = min(1.0 / (light.Attenuation.x * dist * dist + light.Attenuation.y * dist + light.Attenuation.z + 0.000001), 1.0);

                    float NdotL = max(dot(N, L), 0.0);
                    float NdotH = max(dot(N, H), 0.0);

                    vec3 diffuse = NdotL * diffuseColor;
                    vec3 specular = pow(NdotH, shininess) * specularColor;

                    vec3 Lo = (diffuse + specular) * (light.Color.rgb * light.Color.a) * atten;

                    f_Color0 = vec4(Lo, 1.0);
                }
            )"),
        });
        m_PointLightShaderProgram->Bind();
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_POSITION_MAP, RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_NORMAL_MAP, RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ALBEDO_METALLIC_MAP, RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_EMISSIVE_ROUGHNESS_MAP, RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY, RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
        m_PointLightShaderProgram->Unbind();

        m_SpotLightShaderProgram = ShaderProgram::Create("spot_light.program",
        {
            Shader::CreateFromCode("spot_light.vert", Shader::Vertex, R"(
                #version 440 core

                #include "ext/ssbo/CameraSSBO.glsl"

                layout(location = 0) in vec3 a_Pos;
                layout(location = 1) in vec3 a_Normal;
                layout(location = 2) in vec2 a_TexCoord;
                layout(location = 3) in vec3 a_Tangent;

                uniform mat4 u_Model;

                void main()
                {
                    gl_Position = ssbo_Camera.ViewProj * u_Model * vec4(a_Pos, 1.0);
                }
            )"),
            Shader::CreateFromCode("spot_light.frag", Shader::Fragment, R"(
                #version 440 core

                #include "ext/ssbo/CameraSSBO.glsl"
                #include "ext/ssbo/SpotLightSSBO.glsl"

                #define CALC_SHADOW_GLSL_IMPLEMENTATION
                #include "ext/shadow/CalcShadow.glsl"

                layout(location = 0) out vec4 f_Color0;

                uniform int u_Index;
                uniform vec2 u_ScreenSizeInv;

                uniform sampler2D g_PositionMap;
                uniform sampler2D g_NormalMap;
                uniform sampler2D g_AlbedoMetallicMap;
                uniform sampler2D g_EmissiveRoughnessMap;

                void main()
                {
                    vec2 texCoord = gl_FragCoord.xy * u_ScreenSizeInv;

                    vec3 worldPos = texture(g_PositionMap, texCoord).rgb;
                    vec3 N = normalize(texture(g_NormalMap, texCoord).rgb * 2.0 - 1.0);
                    vec3 albedo = texture(g_AlbedoMetallicMap, texCoord).rgb;
                    float metallic = texture(g_AlbedoMetallicMap, texCoord).a;
                    vec3 emissive = texture(g_EmissiveRoughnessMap, texCoord).rgb;
                    float roughness = texture(g_EmissiveRoughnessMap, texCoord).a;

                    vec3 diffuseColor = albedo * clamp(1.0 - metallic, 0.01, 1.0);
                    float shininess = max(pow(1.0 - roughness, 4.0) * 512.0, 16.0);
                    vec3 baseSpecColor = mix(vec3(0.04), albedo, metallic);
                    float specIntensity = mix(0.5, 2.0, metallic) * mix(0.2, 1.0, pow(1.0 - roughness + 0.001, 2.0));
                    vec3 specularColor = baseSpecColor * specIntensity;

                    SpotLight light = ssbo_SpotLight.LightArray[u_Index];

                    vec3 L = normalize(light.Position - worldPos);
                    vec3 V = normalize(ssbo_Camera.CameraPosition - worldPos);
                    vec3 H = normalize(V + L);

                    float dist = length(light.Position - worldPos);
                    float atten = min(1.0 / (light.Attenuation.x * dist * dist + light.Attenuation.y * dist + light.Attenuation.z + 0.000001), 1.0);

                    float lightBeamSize = light.BeamSize;
                    float lightBeamBlend = max(light.BeamBlend, 0.01);

                    float cosineOuter = cos(radians(lightBeamSize));
                    float cosineInner = cos(radians(lightBeamSize * (1.0 - lightBeamBlend)));

                    if (cosineInner <= cosineOuter)
                        discard;

                    float angle = dot(L, normalize(-light.Direction));
                    float spotFactor = clamp((angle - cosineOuter) / (cosineInner - cosineOuter), 0.0, 1.0);

                    float NdotL = max(dot(N, L), 0.0);
                    float NdotH = max(dot(N, H), 0.0);

                    vec3 diffuse = NdotL * diffuseColor;
                    vec3 specular = pow(NdotH, shininess) * specularColor;

                    vec3 Lo = (diffuse + specular) * (light.Color.rgb * light.Color.a) * atten * spotFactor;

                    f_Color0 = vec4(Lo, 1.0);
                }
            )"),
        });
        m_SpotLightShaderProgram->Bind();
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_POSITION_MAP, RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_NORMAL_MAP, RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ALBEDO_METALLIC_MAP, RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_EMISSIVE_ROUGHNESS_MAP, RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::SPOTLIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
        m_SpotLightShaderProgram->Unbind();

        m_FrameBuffer = context.Targets.MainFrameBuffer;

        m_SphereVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_SPHERE);
        m_QuadVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_QUAD);
        m_ConeVAO = ResourceManager::GetGlobal().GetVAO(CommonResources::VAO_CONE);
    }

    void LightVolumeDeferredLightingBlinnPhongRenderPass::Execute(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        // bind the main framebuffer and clear it
        m_FrameBuffer->Bind();
        RenderCommand::Clear();


        BindGeometryMaps(context);
        BindLightMaps(context);

        // light the scene with enviro lights - ambient and directional light
        m_EnvironmentLightShaderProgram->Bind();
        m_QuadVAO->Bind();
        RenderCommand::Draw(*m_QuadVAO);
        m_QuadVAO->Unbind();
        m_EnvironmentLightShaderProgram->Unbind();


        // copy the depth attachment from the geometry framebuffer into this framebuffer
        // this probably should be at the very end, but for debugging I need the depth to be known
        // because I render the wireframe volumes of the lights
        m_FrameBuffer->CopyAttachment(context.Targets.GeometryFrameBuffer, FrameBufferAttachmentType::Depth);


        glDisable(GL_DEPTH_TEST);


        // additive blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);       // additive blending
        glBlendEquation(GL_FUNC_ADD);

        // render only the back face, otherwise the light doubles when both sides render
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        // pointlight
        m_PointLightShaderProgram->Bind();
        m_PointLightShaderProgram->SetFloat2("u_ScreenSizeInv", 1.0f / v2(m_FrameBuffer->GetSpecification().Width, m_FrameBuffer->GetSpecification().Height));
        m_SphereVAO->Bind();

        int index = 0;
        for (const auto& light : context.Lights.PointLights)
        {
            glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(light.Radius));
            glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(light.Position));
            glm::mat4 model = translate * scale;

            m_PointLightShaderProgram->SetMat4("u_Model", model);
            m_PointLightShaderProgram->SetInt("u_Index", index++);

            RenderCommand::Draw(*m_SphereVAO);
        }

        m_SphereVAO->Unbind();
        m_PointLightShaderProgram->Unbind();

        // spotlight
        m_SpotLightShaderProgram->Bind();
        m_SpotLightShaderProgram->SetFloat2("u_ScreenSizeInv", 1.0f / v2(m_FrameBuffer->GetSpecification().Width, m_FrameBuffer->GetSpecification().Height));
        m_ConeVAO->Bind();

        // wireframe rendering
        // glDisable(GL_CULL_FACE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        index = 0;
        for (const auto& light : context.Lights.SpotLights)
        {
            // radius = height * tan(beamSize)
            // height is normalized, because the cone model im using is bounded by a 1x1x1 box
            // this cone's 'beam size' is 26 degrees
            // therefore, scale xz is calculated: tan(beamSize) / tan(26)

            static const float beamAngleDefault = tan(glm::radians(26.0f));
            float scaleXZ = tan(glm::radians(light.BeamSize)) / beamAngleDefault;

            auto model = TransformGroup::Build()
                .Add<Scale>(scaleXZ, 1.0f, scaleXZ)
                .Add<Rotate>(90.0f, v3(1.0, 0.0, 0.0))
                .Add<Rotate>(light.Direction)
                .Add<Scale>(light.Radius)
                .Add<Translate>(light.Position)
                .Compose();

            m_SpotLightShaderProgram->SetMat4("u_Model", model->GetMatrix());
            m_SpotLightShaderProgram->SetInt("u_Index", index++);

            RenderCommand::Draw(*m_ConeVAO);
        }

        // restore from wireframe
        // glEnable(GL_CULL_FACE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        m_ConeVAO->Unbind();
        m_SpotLightShaderProgram->Unbind();



        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        m_FrameBuffer->Unbind();
    }

    void LightVolumeDeferredLightingBlinnPhongRenderPass::BindGeometryMaps(RenderContext& context)
    {
        context.Targets.GeometryPositionMap->BindToSlot(RenderBindingPoints::GEOMETRY_POSITION_MAP);
        context.Targets.GeometryNormalMap->BindToSlot(RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        context.Targets.GeometryAlbedoMetallicMap->BindToSlot(RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        context.Targets.GeometryEmissiveRoughnessMap->BindToSlot(RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        context.Targets.GeometryEntityIDMap->BindToSlot(RenderBindingPoints::GEOMETRY_ENTITY_ID_MAP);
    }

    void LightVolumeDeferredLightingBlinnPhongRenderPass::BindLightMaps(RenderContext& context)
    {
        context.Targets.DirectionalLightShadowMap->BindToSlot(RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);
        context.Targets.SpotLightShadowMapArray->BindToSlot(RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
        context.Targets.PointLightShadowCubeMapArray->BindToSlot(RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
    }
}
