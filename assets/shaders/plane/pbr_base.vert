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
// uniform mat4 MV;

void main()
{
    // World Position of Vertices
    vertex_out.world_Position 	= vec3(M * vec4(aPos, 1.0));
    vertex_out.world_Normals	= mat3(transpose(inverse(M))) * aNormals;
    // UV Per Vertex
    vertex_out.fragTexCoords = aTexCoords;

    gl_Position = P * V * vec4(vertex_out.world_Position, 1.0);
}