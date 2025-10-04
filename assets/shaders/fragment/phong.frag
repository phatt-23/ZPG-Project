#type fragment
#version 400

// enum of light types
const uint LightTypeAmbient      = 1 << 0;
const uint LightTypePoint        = 1 << 1;
const uint LightTypeDirectional  = 1 << 2;
const uint LightTypeSpotlight    = 1 << 3;

struct Light {
    int type;
    vec4 color;
    vec3 pos;
    vec3 dir;
    float inCutoff;
    float outCutoff;
};

uniform Light u_Lights[100];
uniform int u_LightCount;
uniform vec4 u_AmbientColor;
uniform vec3 u_CameraPos;

in vec3 v_WorldPos;
in vec3 v_WorldNormal;
in vec2 v_TexCoord;

out vec4 f_FragColor;


vec3 pointLight(vec3 color, vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor);
vec3 directionalLight(vec3 color, vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor);
vec3 spotLight(vec3 color, vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff);

void main() {

    // calculate the final light color
    int count = min(u_LightCount, u_Lights.length());


    vec3 accumColor = vec3(0.f);
    vec3 color = vec3(1.0, 0.0, 0.0);

    for (int i = 0; i < count; i++) {
        Light light = u_Lights[i];
        
        if (light.type == LightTypeDirectional) {
            accumColor += directionalLight(
                            color, 
                            v_WorldPos, 
                            v_WorldNormal, 
                            light.dir, 
                            vec3(light.color));

        } else if (light.type == LightTypePoint) {
            accumColor += pointLight(
                            color, 
                            v_WorldPos, 
                            v_WorldNormal, 
                            light.pos, 
                            vec3(light.color));


        } else if (light.type == LightTypeSpotlight) {
            accumColor += spotLight(
                            color, 
                            v_WorldPos, 
                            v_WorldNormal, 
                            light.pos, 
                            light.dir, 
                            vec3(light.color), 
                            light.inCutoff);
        }
    }

    // f_FragColor = vec4(v_WorldNormal, 1.f);
    vec3 ambientColor = color * (u_AmbientColor.xyz * u_AmbientColor.w);
    f_FragColor = vec4(ambientColor + accumColor, 1.0);
}



vec3 pointLight(vec3 color, vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightColor) {

    const float specularStrength = 0.4;

    float dist = length(lightPosition - worldPos);
    float attenuation = clamp(5.0 / dist, 0.0, 1.0);

    vec3 viewDir = normalize(u_CameraPos - worldPos);
    vec3 lightDir = normalize(lightPosition - worldPos);
    vec3 reflectionDir = reflect(-lightDir, normalVector);

    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * color * attenuation;
    // vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);

    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }
    vec3 specular = attenuation * spec;

    return diffuse + specular;
}

vec3 spotLight(vec3 color, vec3 worldPos, vec3 normalVector, vec3 lightPosition, vec3 lightDirection, vec3 lightColor, float cutoff) {

    vec3 lightDir = normalize(lightPosition - worldPos);

    float a = 1.0;
    float b = 0.7;
    float distanceFromLight = length(lightPosition - worldPos);
    float attenuation = 1.f / (a * pow(distanceFromLight, 2.f) + b * distanceFromLight + 1.f);

    float theta = dot(lightDir, normalize(-lightDirection));
    if (theta <= cutoff) {
        return vec3(0.0, 0.0, 0.0);
    }

    const float specularStrength = 0.4;

    vec3 viewDir = normalize(u_CameraPos - worldPos);
    vec3 reflectionDir = reflect(-lightDir, normalVector);

    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * color * attenuation;

    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }
    vec3 specular = attenuation * spec;

    return diffuse + specular;
}

vec3 directionalLight(vec3 color, vec3 worldPos, vec3 normalVector, vec3 lightDirection, vec3 lightColor) {

    const float specularStrength = 0.4;

    vec3 lightDir = normalize(-lightDirection);
    float dot_product = dot(lightDir, normalVector);
    vec3 diffuse = max(dot_product, 0.0) * lightColor;

    vec3 viewDir = normalize(u_CameraPos - worldPos);
    vec3 reflectionDir = reflect(-lightDir, normalVector);

    float specValue = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    vec3 spec = specularStrength * specValue * lightColor;
    if (dot_product < 0.0) {
        spec = vec3(0.0);
    }

    return (diffuse + spec) * color;
}

