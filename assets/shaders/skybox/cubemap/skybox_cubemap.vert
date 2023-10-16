#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;

void main()
{
    vec4 pos = P * V * vec4(aPos.x, aPos.y, aPos.z, 1.0); 
    gl_Position = pos.xyww;
    
    TexCoords = vec3(aPos.x, aPos.y, -aPos.z);
}  