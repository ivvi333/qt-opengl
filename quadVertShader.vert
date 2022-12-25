#version 460 core

in vec3 VertexPosition;
in vec3 VertexNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ModelViewProjectionMatrix;

void main()
{
    Normal = normalize(NormalMatrix * VertexNormal);
    Position = vec3(ModelViewMatrix * vec4(VertexPosition, 1.0));

    gl_Position = ModelViewProjectionMatrix * vec4(VertexPosition, 1.0);
}
