#version 330 core

in vec3 Position;
in vec3 Normal;
in vec2 tex;

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;
uniform vec3 lightPos4;
uniform vec3 lightPos5;
uniform vec3 lightPos6;

uniform vec3 viewPos;

uniform vec4 materialEmissive;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float shininess;

uniform sampler2D crateTex;

out vec4 FragColor;

void main(){

    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    FragColor = texture(crateTex, tex);
}
