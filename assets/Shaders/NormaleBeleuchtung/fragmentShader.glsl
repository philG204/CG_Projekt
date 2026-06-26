#version 330 core

#define MAX_LIGHTS 16
#define MAX_OVERLAY_TEXTURES 8

struct Light
{
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

in vec3 Position;
in vec3 Normal;
in vec2 tex;

uniform int lightCount;
uniform Light lights[MAX_LIGHTS];

uniform vec3 viewPos;

uniform vec4 materialEmission;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;

uniform sampler2D baseTexture;
uniform sampler2D overlayTextures[MAX_OVERLAY_TEXTURES];
uniform int overlayTextureCount;

out vec4 FragColor;

void main()
{
    vec4 baseColor = texture(baseTexture, tex);

    if (overlayTextureCount > 0)
    {
        vec4 overlayColor = texture(overlayTextures[0], tex);
        baseColor.rgb = mix(baseColor.rgb, overlayColor.rgb, overlayColor.a);
    }

    if (overlayTextureCount > 1)
    {
        vec4 overlayColor = texture(overlayTextures[1], tex);
        baseColor.rgb = mix(baseColor.rgb, overlayColor.rgb, overlayColor.a);
    }

    if (overlayTextureCount > 2)
    {
        vec4 overlayColor = texture(overlayTextures[2], tex);
        baseColor.rgb = mix(baseColor.rgb, overlayColor.rgb, overlayColor.a);
    }

    if (overlayTextureCount > 3)
    {
        vec4 overlayColor = texture(overlayTextures[3], tex);
        baseColor.rgb = mix(baseColor.rgb, overlayColor.rgb, overlayColor.a);
    }

    vec4 emissive = materialEmission;
    vec4 ambient = vec4(0.0, 0.0, 0.0, 0.0);

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - Position);

    vec4 diffuseSum = vec4(0.0);
    vec4 specularSum = vec4(0.0);

    int count = min(lightCount, MAX_LIGHTS);

    for (int i = 0; i < count; i++)
    {
        ambient = materialAmbient * lights[i].ambient;

        vec3 L = normalize(lights[i].position - Position);
        vec3 H = normalize(L + V);

        float diff = max(dot(N, L), 0.0);

        diffuseSum += diff * materialDiffuse * lights[i].diffuse;

        if (diff > 0.0)
        {
            float spec = pow(max(dot(N, H), 0.0), materialShininess);
            specularSum += spec * materialSpecular * lights[i].specular;
        }
    }

    vec4 color = emissive
               + ambient * baseColor
               + diffuseSum * baseColor
               + specularSum;

    FragColor = vec4(color.rgb, baseColor.a * materialDiffuse.a);
}