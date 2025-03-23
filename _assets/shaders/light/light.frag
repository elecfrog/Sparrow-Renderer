#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 color;

uniform vec3 camPos;

const float PI = 3.14159265359;

void main()
{
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(lightPosition - WorldPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(camPos - WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * lightColor;

    vec3 ambient = 0.1 * lightColor;

    vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.0);
}