#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aTangents;
layout (location = 3) in vec2 aTexCoords;
layout (location = 4) in vec4 boneIDs;
layout (location = 5) in vec4 weights;

out vec3 fragNormals;
out vec2 fragTexCoords;
out vec3 vert_worldPos;
out mat3 TBN;

uniform mat4 M; // Model Matrix
uniform mat4 V; // View  Matrix
uniform mat4 P; // Proj  Matrix



// out vec3 tangentVertPos;
// out vec3 tangentLightPos;
// out vec3 tangentViewPos;

void main()
{
	gl_Position = P * V * M * vec4(aPos, 1.0);
	vert_worldPos = vec3(M * vec4(aPos, 1.0));
	fragTexCoords = aTexCoords;


 	mat3 normalMatrix = transpose(inverse(mat3(M)));
	vec3 N = normalize(normalMatrix * aNormals); 	// normal
	vec3 T = normalize(normalMatrix * aTangents); 	// tangent
	vec3 B = cross(T, N);							// bitangent
	TBN = mat3(T, B, N);						// TBN Matrix in world space

    // tangentLightPos  = TBN * lightPos;
	// tangentVertPos = vert_worldPos;
    // tangentViewPos  = TBN * viewPos;
}