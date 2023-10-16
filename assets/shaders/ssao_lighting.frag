#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gColor;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

void main()
{
    // retrieve data from gbuffer
    vec3 color = texture(gColor, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedo, TexCoords).rgb;
    float AmbientOcclusion = texture(ssao, TexCoords).r;

    // then calculate lighting as usual
    vec3 ambient = vec3(0.3 * Diffuse * AmbientOcclusion);

    FragColor = vec4(ambient + color, 1.0);
}
