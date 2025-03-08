#version 450 core

// Ouput Data, the only is color
out vec4 color;

in VERTEX_OUT
{
	vec3 view_Normal;
	vec2 fragTexCoords;

	vec3 world_Position;
	vec3 world_Normals;
} vertex_in;

// Lighting Variables
struct LIGHT
{
	vec3 position;
	vec3 specularColor;
	vec3 diffuseColor;
	vec3 ambientColor;	
};
uniform LIGHT light;

struct MATERIAL
{
	vec4 baseColor;
};
uniform MATERIAL material;


uniform vec3 viewPos;

// Texturess
uniform sampler2D tex_Diffuse;

void main()
{
	vec3 norm = normalize(vertex_in.world_Normals);

	vec3 lightDir = normalize(light.position - vertex_in.world_Position);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuseColor;

	vec3 viewDir = normalize(viewPos - vertex_in.world_Position);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
	vec3 specular = spec * vec3(1.0);

	vec3 ambient = 0.1 * light.diffuseColor;

	vec3 result = (ambient + diffuse + specular) * vec3(material.baseColor);
	
//	vec3 ambinetContribution = light.ambientColor;
	color = vec4(result, 1.0);
}