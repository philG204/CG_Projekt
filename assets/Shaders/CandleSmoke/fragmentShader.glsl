#version 330 core

in vec2 TexCoord;
in vec3 WorldPos;

uniform float time;

out vec4 FragColor;

float rand(vec2 p)
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float noise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = rand(i);
    float b = rand(i + vec2(1.0, 0.0));
    float c = rand(i + vec2(0.0, 1.0));
    float d = rand(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x)
         + (c - a) * u.y * (1.0 - u.x)
         + (d - b) * u.x * u.y;
}

void main()
{
    vec2 uv = TexCoord;

    // aufsteigende Bewegung
    uv.y -= time * 0.35;

    // leichtes seitliches Flackern / Wabern
    uv.x += sin(TexCoord.y * 10.0 + time * 2.0) * 0.05;

    float n1 = noise(uv * 4.0);
    float n2 = noise(uv * 8.0 + vec2(2.3, 0.7));
    float smoke = mix(n1, n2, 0.5);

    // Form des Rauchs
    float centerFade = 1.0 - smoothstep(0.12, 0.45, abs(TexCoord.x - 0.5));
    float bottomFade = smoothstep(0.0, 0.08, TexCoord.y);
    float topFade    = 1.0 - smoothstep(0.55, 1.0, TexCoord.y);

    float alpha = smoke * centerFade * bottomFade * topFade * 0.55;

    if (alpha < 0.03)
        discard;

    vec3 smokeColor = vec3(0.55, 0.55, 0.55);

    FragColor = vec4(smokeColor, alpha);
}