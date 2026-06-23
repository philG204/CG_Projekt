#version 330 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 normalM;

out vec3 Position;
out vec3 Normal;
out vec2 tex;

void main(){
    vec4 pos = vec4(view * model * vec4(vPosition, 1.0));
    Normal = mat3(view * model) * vNormal;

    Position = pos.xyz / pos.w;

    tex = vTexCoord;

    gl_Position = proj * view * model * vec4(vPosition, 1.0);
}