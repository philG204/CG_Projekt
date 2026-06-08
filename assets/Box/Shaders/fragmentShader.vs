#version 330 core

in vec3 Position;
in vec3 Normal;
in vec2 tex;

uniform sampler2D earthDay;
uniform sampler2D earthNight;
uniform sampler2D earthOcean;
uniform sampler2D earthClouds;

uniform vec3 lightPos;
uniform float time;

out vec4 FragColor;

void main()
{
    vec2 uv = tex;
    uv.y = 1.0 - uv.y;

    vec3 N = normalize(Normal);
    vec3 L = normalize(lightPos - Position);

    float light = max(dot(N, L), 0.0);

    vec4 dayColor = texture(earthDay, uv);
    vec4 nightColor = texture(earthNight, uv);

    vec4 earthColor = mix(nightColor, dayColor, light);

    // View-Richtung grob aus Kameraposition im View/World-Space.
    // Für deine Aufgabe reicht das erstmal.
    vec3 V = normalize(-Position);
    vec3 R = reflect(-L, N);

    float spec = pow(max(dot(V, R), 0.0), 64.0);

    float oceanValue = texture(earthOcean, uv).r;

    // Wenn Land glänzt statt Wasser, diese Zeile wechseln:
    float oceanMask = step(0.5, oceanValue);
    // float oceanMask = step(0.5, 1.0 - oceanValue);

    vec3 specular = vec3(0.8 * spec * oceanMask * light);

    earthColor.rgb += specular;

    vec4 cloudTex = texture(earthClouds, vec2(tex.x + time, tex.y));

    // Wichtig:
    // Nicht cloudTex.a benutzen, wenn die Textur keinen echten Alpha-Kanal hat.
    // Stattdessen Helligkeit der Wolkentextur als Alpha verwenden.
    float cloudAlpha = cloudTex.r;

    // Stärke der Wolken etwas reduzieren
    cloudAlpha *= 0.6;

    // Wolkenfarbe weiß machen, nicht grau/schwarz aus der Textur übernehmen
    vec4 cloudColor = vec4(1.0, 1.0, 1.0, 1.0);

    vec4 finalColor = mix(earthColor, cloudColor, cloudAlpha);

    FragColor = finalColor;
}