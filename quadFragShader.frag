#version 460 core
#define MAX_LIGHT_COUNT 4

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D tex;

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

out vec4 FragColor;

void ads(int lightIndex, vec3 position, vec3 norm, out vec3 ambAndDiff, out vec3 spec)
{
    vec3 s = normalize(lights[lightIndex].Position - position);
    vec3 v = normalize(-position);
    vec3 r = reflect(-s, norm);
    float distance = length(lights[lightIndex].Position - position);
    float attenuation = 1.0 / (lights[lightIndex].constant + lights[lightIndex].linear * distance +
                               lights[lightIndex].quadratic * (distance * distance));
    ambAndDiff += attenuation * lights[lightIndex].Intensity * (Ka + Kd * max(dot(s, norm), 0.0));
    spec += attenuation * lights[lightIndex].Intensity * Ks * pow(max(dot(r,v), 0.0), Shininess);
}

void main(void)
{
    vec3 ambAndDiff = vec3(0.0), spec = vec3(0.0);
    vec4 texColor = texture(tex, TexCoord);
    for(int i = 0; i < LightCount; i++)
        ads(i, Position, Normal, ambAndDiff, spec);
    FragColor = vec4(ambAndDiff, 1.0) * texColor + vec4(spec, 1.0);
}
