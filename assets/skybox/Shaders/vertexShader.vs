#version 330 core
layout (location = 0) in vec3 vPosition;
//layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexture;

out vec3 Position;

uniform mat4 model;
uniform mat4 viewProj;

void main() {
    Position = aPosition;
    vec4 pos = viewProj * model * vec4(vPosition, 1.0);
    gl_Position = pos.xyww;
}
