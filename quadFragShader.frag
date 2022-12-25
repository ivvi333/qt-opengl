#version 460 core
#define MAX_LIGHT_COUNT 4

in vec3 Position;
in vec3 Normal;

uniform vec3 ObjectColor;

out vec4 FragColor;

struct LightInfo
{
    vec3 Position;
    vec3 Intensity;
    float constant;
    float linear;
    float quadratic;
};

uniform int LightCount;
uniform LightInfo lights[MAX_LIGHT_COUNT];
uniform vec3 Kd;
uniform vec3 Ka;
uniform vec3 Ks;
uniform float Shininess;

vec3 ads(int lightIndex, vec3 position, vec3 norm)
{
    vec3 s = normalize(lights[lightIndex].Position - position);
    vec3 v = normalize(-position);
    vec3 r = reflect(-s, norm);
    float distance = length(lights[lightIndex].Position - position);
    float attenuation = 1.0 / (lights[lightIndex].constant + lights[lightIndex].linear * distance +
                               lights[lightIndex].quadratic * (distance * distance));
    return attenuation * lights[lightIndex].Intensity *
           (Ka + Kd * max(dot(s, norm), 0.0) + Ks * pow(max(dot(r,v), 0.0), Shininess));
}

void main(void)
{
    vec3 Color = vec3(0.0);
    for(int i = 0; i < LightCount; i++)
        Color += ads(i, Position, Normal);
    FragColor = vec4(ObjectColor * Color, 1.0);
}
