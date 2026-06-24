#version 330 core

in vec2 TexCoord;

uniform sampler2D screenTexture;

out vec4 FragColor;

void main()
{
    vec4 color = texture(screenTexture, TexCoord);

    // invert sample
    FragColor = vec4(1.0 - color.rgb, color.a);
}