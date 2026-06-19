#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 viewProj;

out vec3 Position;
out vec3 Normal;
out vec2 tex;

void main(){
    Position = vec3(model * vec4(vPosition, 1.0));
    Normal = mat3(model) * vNormal;

    tex = vTexCoord;

    gl_Position = viewProj * model * vec4(vPosition, 1.0);
}