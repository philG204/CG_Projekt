#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoord;
out vec3 WorldPos;

void main()
{
    vec4 world = model * vec4(vPosition, 1.0);
    WorldPos = world.xyz;
    TexCoord = vTexCoord;

    gl_Position = proj * view * world;
}