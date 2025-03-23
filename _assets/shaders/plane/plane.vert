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

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoords;

out VERTEX_OUT
{
	vec3 view_Normal;
	vec2 fragTexCoords;

	vec3 world_Position;
	vec3 world_Normals;
} vertex_out;

void main()
{
	// World Position of Vertices
	vertex_out.world_Position = vec3(model * vec4(aPos, 1.0));
	vertex_out.world_Normals = mat3(transpose(inverse(model))) * aNormals;
	// UV Per Vertex
	vertex_out.fragTexCoords = aTexCoords;

	gl_Position = projection * view * vec4(vertex_out.world_Position, 1.0);
}