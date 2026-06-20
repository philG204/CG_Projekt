#version 330 core

in vec3 Position;
in vec3 Normal;
in vec2 tex;

uniform sampler2D earthDay;
uniform sampler2D earthClouds;

out vec4 FragColor;

void main(){

    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    FragColor = texture(earthDay, tex);
}
