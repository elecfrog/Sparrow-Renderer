#version 330 core
uniform sampler2D bloomBlur;
uniform sampler2D gColor;
uniform sampler2D gAlbedo;
uniform sampler2D gNormal;
uniform sampler2D ssao;
uniform samplerCube irradianceMap;

uniform bool isSsao;
uniform bool bloom;
uniform bool ibl;
in vec2 TexCoords;
out vec4 FragColor;

void main()
{
    // retrieve data from gbuffer
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 color = texture(gColor, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
    float AmbientOcclusion = texture(ssao, TexCoords).r;
    vec3 ambient;
    if (ibl)
    ambient = Diffuse* irradiance;
    else
    ambient = Diffuse;

    // then calculate lighting as usual
    if (isSsao)
    ambient *= 0.3 * AmbientOcclusion;

    vec3 finalColor = ambient + color;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    if (bloom)
    {
        finalColor += bloomColor;// additive blending
    }

    FragColor = vec4(finalColor, 1.0);
}
