#type fragment
#version 400

// enum of light types
const uint LightTypeAmbient      = 1 << 0;
const uint LightTypePoint        = 1 << 1;
const uint LightTypeDirectional  = 1 << 2;
const uint LightTypeSpotlight    = 1 << 3;

struct Light {
    int Type;
    vec4 Color;
    vec3 Pos;
    vec3 Dir;
    float BeamSize;
    float BeamBlend;
};

uniform Light u_Lights[100];
uniform int u_LightCount;
uniform vec4 u_AmbientColor;
uniform vec3 u_CameraPos;

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;

out vec4 f_FragColor;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_NormalMap;

vec3 pointLight(
    vec3 p_Color, 
    vec3 p_Normal, 
    vec3 p_LightPos, 
    vec3 p_LightColor);

vec3 directionalLight(
    vec3 p_Color, 
    vec3 p_nNormal, 
    vec3 p_LightDir, 
    vec3 p_LightColor);

vec3 spotLight(
    vec3 p_Color, 
    vec3 p_Normal, 
    vec3 p_LightPos, 
    vec3 p_LightDir, 
    vec3 p_LightColor, 
    float p_Cutoff);

void main() {
    // calculate the final light color
    int count = min(u_LightCount, u_Lights.length());

    vec3 accumColor = vec3(0.f);
    vec3 color = vec3(texture(u_AlbedoMap, v_TexCoord));

    for (int i = 0; i < count; i++) {
        Light light = u_Lights[i];
        
        if (light.Type == LightTypeDirectional) {
            accumColor += directionalLight(
                color, 
                v_WorldNormal, 
                light.Dir, 
                light.Color.xyz * light.Color.w);

        } else if (light.Type == LightTypePoint) {
            accumColor += pointLight(
                color, 
                v_WorldNormal, 
                light.Pos, 
                light.Color.xyz * light.Color.w);

        } else if (light.Type == LightTypeSpotlight) {
            accumColor += spotLight(
                color, 
                v_WorldNormal, 
                light.Pos, 
                light.Dir, 
                light.Color.xyz * light.Color.w, 
                light.BeamSize);
        }
    }

    vec3 ambientColor = color * (u_AmbientColor.xyz * u_AmbientColor.w);
    f_FragColor = vec4(ambientColor + accumColor, 1.0);
}

vec3 pointLight(
    vec3 p_Color, 
    vec3 p_Normal, 
    vec3 p_LightPos, 
    vec3 p_LightColor
) {
    float dist = length(p_LightPos - v_WorldPos);
    float attenuation = 5.0 / (dist * dist);

    vec3 viewDir = normalize(u_CameraPos - v_WorldPos);
    vec3 lightDir = normalize(p_LightPos - v_WorldPos);
    vec3 reflectionDir = reflect(-lightDir, p_Normal);

    float orthogonality = dot(lightDir, p_Normal);
    vec3 diffuse = max(orthogonality, 0.0) * p_Color * attenuation;

    const float specularStrength = 0.4;
    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    float specFactor = specularStrength * specValue;
    if (orthogonality < 0.0) {
        specFactor = 0.0;
    }

    vec3 specular = vec3(texture(u_NormalMap, v_TexCoord)) * attenuation * specFactor;
    return diffuse + specular;
}


vec3 spotLight(
    vec3 p_Color, 
    vec3 p_Normal, 
    vec3 p_LightPos, 
    vec3 p_LightDir, 
    vec3 p_LightColor, 
    float p_BeamSize
) {
    vec3 lightDir = normalize(p_LightPos - v_WorldPos);

    float a = 1.0;
    float b = 0.7;
    float dist = length(p_LightPos - v_WorldPos);
    float attenuation = 1.f / (a * pow(dist, 2.f) + b * dist + 1.f);

    float theta = dot(lightDir, normalize(-p_LightDir));
    if (theta <= p_BeamSize) {
        return vec3(0.0);
    }

    const float specularStrength = 0.4;

    vec3 viewDir = normalize(u_CameraPos - v_WorldPos);
    vec3 reflectionDir = reflect(-lightDir, p_Normal);

    float orthogonality = dot(lightDir, p_Normal);
    vec3 diffuse = max(orthogonality, 0.0) * p_Color * attenuation 
                 + p_LightColor * attenuation;

    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 specFactor = specularStrength * specValue * p_LightColor;

    if (orthogonality < 0.0) {
        specFactor = vec3(0.0);
    }

    vec3 specular = attenuation * specFactor;

    return diffuse + specular;
}

vec3 directionalLight(
    vec3 p_Color, 
    vec3 p_Normal, 
    vec3 p_LightDir, 
    vec3 p_LightColor
) {
    const float specularStrength = 0.4;

    vec3 lightDir = normalize(-p_LightDir);
    float orthogonality = dot(lightDir, p_Normal);
    vec3 diffuse = max(orthogonality, 0.0) * p_LightColor;

    vec3 viewDir = normalize(u_CameraPos - v_WorldPos);
    vec3 reflectionDir = reflect(-lightDir, p_Normal);

    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 specFactor = specularStrength * specValue * p_LightColor;
    if (orthogonality < 0.0) {
        specFactor = vec3(0.0);
    }

    return (diffuse + specFactor) * p_Color;
}

