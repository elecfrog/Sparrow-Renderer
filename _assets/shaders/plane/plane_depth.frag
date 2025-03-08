#version 450 core

// Ouput Data, the only is color
// out vec4 color;

in VERTEX_OUT
{
	vec3 view_Normal;
	vec2 fragTexCoords;

	vec3 world_Position;
	vec3 world_Normals;
} vertex_in;

// Lighting Variables
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// Textures
uniform sampler2D tex_Diffuse;


vec3 basic_lighting()
{
	// Texture properties
	vec3 TextureDiffuseColor  = texture(tex_Diffuse, vertex_in.fragTexCoords).rgb;

	// Material properties
	vec3 MaterialAmbientColor  = vec3(0.2f, 0.1f, 0.2f);
	vec3 MaterialDiffuseColor  = vec3(1.0f, 1.0f, 1.0f);
	vec3 MaterialSpecularColor = vec3(1.0f, 1.0f, 1.0f);

	// ambient lighting
	vec3 ambient = MaterialAmbientColor * lightColor;

	// diffuse lighting
	vec3 norm = normalize(vertex_in.world_Normals);
	vec3 lightDir = normalize(lightPos - vertex_in.world_Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = MaterialDiffuseColor * diff * lightColor;

	// specular lighting
	vec3 viewDir = normalize(viewPos - vertex_in.world_Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = MaterialSpecularColor * spec * lightColor;

	return (ambient + diffuse + specular) * TextureDiffuseColor;
}

void main()
{
//	color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	// color = vec4(basic_lighting(),1.0f);
}