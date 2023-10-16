#version 450 core
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

uniform mat4 M; // Model Matrix
uniform mat4 V; // View  Matrix
uniform mat4 P; // Proj  Matrix
uniform mat4 MV;

uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = lightSpaceMatrix* M * vec4(aPos.x, aPos.y, aPos.z, 1.0);

	// World Position of Vertices
	vertex_out.world_Position 	= vec3(M * vec4(aPos.x, aPos.y, aPos.z, 1.0));
	vertex_out.world_Normals	= mat3(transpose(inverse(M))) * aNormals;


	// Normal Per Vertex
	vertex_out.view_Normal = mat3(MV) * aNormals;

	// UV Per Vertex
	vertex_out.fragTexCoords = aTexCoords;
}