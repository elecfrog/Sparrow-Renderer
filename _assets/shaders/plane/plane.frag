#version 450 core

// UBO绑定点定义
layout(std140, binding = 0) uniform Matrices
{
	mat4 model;
	mat4 view;
	mat4 projection;
};

layout(std140, binding = 1) uniform Camera
{
	vec3 cameraPos;
	float padding;
};

layout(std140, binding = 2) uniform Light
{
	vec3 lightPos;
	float padding1;
	vec3 lightColor;
	float padding2;
	vec3 ambientColor;
	float padding3;
};

// Ouput Data, the only is color
out vec4 color;

in VERTEX_OUT
{
	vec3 view_Normal;
	vec2 fragTexCoords;

	vec3 world_Position;
	vec3 world_Normals;
} vertex_in;

// Material
struct MATERIAL
{
	vec4 baseColor;
};
uniform MATERIAL material;

// Textures
uniform sampler2D tex_Diffuse;

void main()
{
	vec3 norm = normalize(vertex_in.world_Normals);

	vec3 lightDir = normalize(lightPos - vertex_in.world_Position);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(cameraPos - vertex_in.world_Position);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
	vec3 specular = spec * vec3(1.0);

	vec3 ambient = 0.1 * lightColor;

	vec3 result = (ambient + diffuse + specular) * vec3(material.baseColor);
	
	color = vec4(result, 1.0);
}