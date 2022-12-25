#version 460 core

in vec4 VertexPosition;
in vec3 VertexNormal;

out vec4 Position;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewProjectionMatrix;

void main()
{
    Normal = normalize(NormalMatrix * VertexNormal);
    Position = ModelViewMatrix * VertexPosition;

    gl_Position = ModelViewProjectionMatrix * VertexPosition;
}
