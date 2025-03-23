#version 450 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertPosition_modelspace;
layout(location = 1) in vec2 vertUV;
layout(location = 2) in vec3 vertNormal_modelspace;

// Output data; will be interpolated for each fragment.
out vec2 tescUV;
out vec3 tescNormal_modelspace;

void main(){
	// Only Pass the neccessary Information Foward
	gl_Position = vec4(vertPosition_modelspace, 1);
	tescUV = vertUV;
	tescNormal_modelspace = vertNormal_modelspace;
}


