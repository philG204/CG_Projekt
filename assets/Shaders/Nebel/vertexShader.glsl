#version 330

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalM;

out vec3 Position;
out vec3 Normal;
out vec2 tex;

void main(){
    vec4 pos = vec4(model * vec4(vPosition, 1.0));
    
    Position = pos.xyz / pos.w;
    Normal = normalize(transpose(inverse(mat3(model))) * vNormal);
    tex = vTexCoord;

    gl_Position = proj * view * model * vec4(vPosition, 1.0);
}