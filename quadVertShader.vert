#version 460 core

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewProjectionMatrix;

void main()
{
    TexCoord = VertexTexCoord;
    Normal = normalize(NormalMatrix * VertexNormal);
    Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));

    gl_Position = ModelViewProjectionMatrix * vec4(VertexPosition, 1.0);
}
