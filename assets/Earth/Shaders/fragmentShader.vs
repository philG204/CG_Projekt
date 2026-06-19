#version 330 core

in vec3 Position;
in vec3 Normal;
in vec2 tex;

uniform sampler2D earth_day.png;

out vec4 FragColor;

void main(){

    //FragColor = texture(earth_day.png, tex);
    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}