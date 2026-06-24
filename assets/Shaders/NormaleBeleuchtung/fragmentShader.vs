#version 330 core

#define MAX_LIGHTS 16

in vec3 Position;
in vec3 Normal;
in vec2 tex;

uniform int lightCount;
uniform vec3 lightPos[MAX_LIGHTS];

uniform vec3 viewPos;

uniform vec4 materialEmission;
uniform vec4 materialAmbient;
uniform vec4 materialDiffuse;
uniform vec4 materialSpecular;
uniform float materialShininess;

uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;

uniform sampler2D crateTex;

out vec4 FragColor;

void main(){
    vec4 emissiv = materialEmission;
    vec4 ambient = materialAmbient * lightAmbient;

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - Position);

    vec4 diffuseSum  = vec4(0.0);
    vec4 specularSum = vec4(0.0);

    int count = min(lightCount, MAX_LIGHTS);

    for (int i = 0; i < count; i++)
    {
        vec3 L = normalize(lightPos[i] - Position);
        vec3 H = normalize(L + V);

        float diff = max(dot(N, L), 0.0);

        diffuseSum += diff * materialDiffuse * lightDiffuse;

        if (diff > 0.0)
        {
            float spec = pow(max(dot(N, H), 0.0), materialShininess);
            specularSum += spec * materialSpecular * lightSpecular;
        }
    }

    vec4 baseColor = texture(crateTex, tex);

    //FragColor = vec4(1.0, 1.0, 0.0, 1.0);
    FragColor = emissiv + (ambient) + (diffuseSum) + specularSum;
    //FragColor = texture(crateTex, tex);
}
