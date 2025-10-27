#pragma once

namespace ZPG {

class ShaderProgram;
class Texture;

/**
 * Can either be texture based or single factor based.
 */

class Material {
public:
    using UniformName = std::string;
    template<typename T, typename S> using Map = std::unordered_map<T, S>;

    // assigns defaults
    Material(const std::string& name = "");
    ~Material();

    void SetShaderProgram(const ref<ShaderProgram>& shaderProgram);
    ref<ShaderProgram>& GetShaderProgram();

    void SetAlbedo(const v4& albedo);
    void SetRoughness(f32 roughness);
    void SetMetallic(f32 metallic);
    void SetEmissive(const v4& emissive);

    const v4& GetAlbedo() const;
    f32 GetRoughness() const;
    f32 GetMetallic() const;
    const v4& GetEmissive() const;

    void SetAlbedoMap(const ref<Texture>& albedoMap);
    void SetNormalMap(const ref<Texture>& normalMap);
    void SetMetalnessMap(const ref<Texture>& metalnessMap);
    void SetRoughnessMap(const ref<Texture>& roughnessMap);
    void SetEmissiveMap(const ref<Texture>& emissiveMap);

    const ref<Texture>& GetAlbedoMap() const;
    const ref<Texture>& GetNormalMap() const;
    const ref<Texture>& GetMetalnessMap() const;
    const ref<Texture>& GetRoughnessMap() const;
    const ref<Texture>& GetEmissiveMap() const;

    void SetName(const std::string& name);
    std::string const& GetName() const;

public:
    std::string m_Name;

    ref<ShaderProgram> m_ShaderProgram;  // by which this material is drawn

    /**
     * Base Color factor.
     * By default, it is (1,1,1), so it doesn't affect the albedo map.
     */
    v4 m_Albedo;

    /**
     * Micro-surface factor.
     * Dictates how much the light rays are diffused. 
     * 0 - Brighter and tight specular reflections, 1 - Wider and dimmer specular reflections.
     */
    f32 m_Roughness;

    /**
     * Reflectivity factor.
     */
    f32 m_Metallic;

    // Material's glow.
    v4 m_Emissive;

    /**
     * Color map, base color map, diffuse map.
     *
     * In shader code it is used like this:
     *
     *     vec3 color = clamp(texture(u_AlbedoMap, v_TexCoord).rgb * u_AlbedoColor, vec3(0.01), vec3(1.0));
     *
     * When none is provided, then it is set to 'null' albedo map,
     * which is a 1x1 texture with single color (1,1,1).
     * When multiplied by the albedo color, it has no effect
     */
    ref<Texture> m_AlbedoMap;

    /**
     * Metalness map. Reflectivity input.
     *
     *     float metallic = clamp(texture(u_MetalnessMap, v_TexCoord).r * u_Metallic, 0.01, 1.0);
     */
    ref<Texture> m_MetalnessMap;

    /**
     * Roughness map. Micro-surface input.
     *
     *     float roughness = clamp(texture(u_RoughnessMap, v_TexCoord).r * u_Metallic, 0.01, 1.0);
     */
    ref<Texture> m_RoughnessMap;

    /**
     * By default, it is 1x1 texture with color (0.5, 0.5, 1.0).
     *
     *     vec3 normal = normalize(v_TBN * (texture(u_NormalMap, v_TexCoord).rgb * 2.0 - 1.0));
     */
    ref<Texture> m_NormalMap;

    /**
     * Emissive map, by default (1, 1, 1)
     */
    ref<Texture> m_EmissiveMap;
};

}

// RAMBLING

 /**
       Normal map encodes surface normal directions in RGB in range of [0, 1],
       while 3D models live in -1, 1 space.

       By default, it is 1x1 texture with one color (0.5, 0.5, 1.0).

       A flat normal (pointing straight out) has value (0,0,1), in RGB encoded as (0.5, 0.5, 1.0).
       Why not 0,0,1?

       A texture’s RGB channels store values in the range [0, 1],
       but a 3D normal vector has components that can be negative (for directions pointing left, down, ...), in [−1, 1].

       Need to remap - bijective mapping from [-1, 1] -> [0, 1].


       The normal in a normal map are stored in tangent space. They are 'local' to a specific primitive (triangle).
       That's why they are mostly blue.

       The retrieved normal has to be transformed into world space through TBN matrix.
       Tangent, Bitangent and Normal vectors are needed to create TBN. They are all perpendicular
       to one another and are aligned along the surface of the normal map: N - up, T - right, B - forward.
       The TBN is an orthogonal matrix: inverse(TBN) == transpose(TBN).

       vec3 T = normalize(vec3(u_Model * vec4(a_Tangent,   0.0)));
       vec3 B = normalize(vec3(u_Model * vec4(a_Bitangent, 0.0)));
       vec3 N = normalize(vec3(u_Model * vec4(a_Normal,    0.0)));
       mat3 TBN = mat3(T, B, N);

       OR

       mat3 normalMat = normalize(transpose(inverse(mat3(u_Model)));
       vec3 T = normalize(normalMat * a_Tangent);
       vec3 B = normalize(normalMat * a_Bitangent);
       vec3 N = normalize(normalMat * a_Normal);
       mat3 TBN = mat3(T, B, N);

       Send the TBN to fragment shader.

       In shader code, it is used like:
           vec3 normal = texture(u_NormalMap, v_TexCoords).rgb;  // starting with [0, 1] range
           normal = encoded * 2.0 - 1.0;                         // map to [-1, 1] range
           normal = normalize(decoded);                          // because mapping can cause artifacts
           normal = normalize(TBN * normal)

       To optimize this, in vertex shader we can send vertices from vertex
       to fragment in tangent space and not world space. This way matrix multiplication
       of TBN with the normal (retrieved from normal map) can be eliminated.
       We have to transform every object into the tangent space in the vertex shader, send that to the fragment.
       Fragment does the usual calculation, but doesn't have to have additional matrix multiply for each fragment.
       Vertex shader:
            mat3 TBN = transpose(mat3(T, B, N));
            vs_out.TangentLightPos = TBN * lightPos;
            vs_out.TangentViewPos  = TBN * viewPos;
            vs_out.TangentFragPos  = TBN * vec3(model * vec4(aPos, 1.0));
            ... and so on for every entity/object

      Don't need the Bitangent, T could be non-orthogonal after transformation, can be re-orthogonalize.
        vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
        vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

        // re-orthogonalize T with respect to N
        T = normalize(T - dot(T, N) * N);

        // then retrieve perpendicular vector B with the cross product of T and N
        vec3 B = cross(N, T);

        mat3 TBN = mat3(T, B, N)
    */
