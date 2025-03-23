#version 450 core

layout (vertices = 4 ) out;

// InputData: Origianal Vertex Infomation from VertexShader.
in vec2 tescUV[];
in vec3 tescNormal_modelspace[];

// Output data ; will be interpolated for each fragment.
out vec2 tevaUV[];
out vec3 tevaNormal_modelspace[];

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tevaUV[gl_InvocationID] = tescUV[gl_InvocationID];
    tevaNormal_modelspace[gl_InvocationID] = tescNormal_modelspace[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 8; 
        gl_TessLevelOuter[1] = 8; 
        gl_TessLevelOuter[2] = 8; 
        gl_TessLevelOuter[3] = 8; 

        gl_TessLevelInner[0] = 8; 
        gl_TessLevelInner[1] = 8; 
    }
}