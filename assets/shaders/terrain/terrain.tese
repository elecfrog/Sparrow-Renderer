#version 450 core
layout (quads, fractional_even_spacing, ccw) in;

// Input Data, Comes from TCSShader.
in vec2 tevaUV[];
in vec3 tevaNormal_modelspace[];

// Output Data,
out TESE_DATA
{
	out vec2 UV;
	out vec3 Position_worldspace;
	out vec3 EyeDirection_cameraspace;
	out vec3 LightDirection_cameraspace;
	out vec3 Normal_cameraspace;
	out vec3 tex_radio;
}teseOut;

// Uniform Variables

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition_worldspace;

// Values that stay constant for the whole mesh.
uniform sampler2D DiffuseTextureSampler;

// Using Bit Opertions to Get Real Height Value From [0,255] RGB Value
float compute_height(vec3 RGBValue, float scale, float shift)
{
	// Simple Bit Opertions for Each Channel. 
	// And Sum them to Construct the Original Value.
	int height = int(int(RGBValue.r) << 16) + int(int(RGBValue.g) << 8)  + int(RGBValue.b);
	// int height = int(int(RGBValue.r) << 16) + int(int(RGBValue.g) << 8) ;
	// Using Bit Opertions to Get Real Height Value From [0,255] RGB Value
	// Apply with constant scale zooming the coordinate ajustion for easy to rendering result.
	return scale * float(height) + shift;
}

vec2 CheckBoundary(vec2 _p)
{
	vec2 res = _p; 
	if(_p.x < 0.0f ) res.x = 0.0f;
	else if ( _p.x > 1.0f) res.x = 1.0f;
	else if ( _p.y < 0.0f) res.y = 0.0f;
	else if ( _p.y > 1.0f) res.y = 1.0f;
	return res;
}

void main()
{
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 uv0 = tevaUV[0];
    vec2 uv1 = tevaUV[1];
    vec2 uv2 = tevaUV[2];
    vec2 uv3 = tevaUV[3];

    vec2 leftUV = uv0 + v * (uv3 - uv0);
    vec2 rightUV = uv1 + v * (uv2 - uv1);
    vec2 texCoord = leftUV + u * (rightUV - leftUV);    // This is current UV we want!

	// Get Value of each vertex from Height Map
	// rgb is in [0,1], So We need to convert to [0,255]
	// Apply with constant scale zooming the coordinate ajustion for easy to rendering result.
	float y_scale = 0.00002f;
	float y_shift = -50.0f;
	float y_length = 10.0f;
	vec3 heightRGB = texture(DiffuseTextureSampler, vec2(texCoord.x, texCoord.y)).rgb * 255.0f;
	float real_height = compute_height(heightRGB, y_scale, y_shift);

    vec4 pos0 = gl_in[0].gl_Position;
    vec4 pos1 = gl_in[1].gl_Position;
    vec4 pos2 = gl_in[2].gl_Position;
    vec4 pos3 = gl_in[3].gl_Position;

    vec4 leftPos = pos0 + v * (pos3 - pos0);
    vec4 rightPos = pos1 + v * (pos2 - pos1);
    vec4 pos = leftPos + u * (rightPos - leftPos); // This is the position we Want!

    gl_Position = MVP * vec4(pos.x, real_height, pos.z, 1.0f); // Matrix transformations go here

    teseOut.tex_radio = vec3(0,0,0);

    float upheight = real_height - y_shift;
	if( real_height > -50.0f && real_height < -20.0f)
	{
		teseOut.tex_radio.x = (1 - upheight/30.0f) + 0.2f;
		teseOut.tex_radio.y = upheight/30.0f - 0.2f;
		teseOut.tex_radio.z = 0;
	}
	else if( real_height >= -20.0f && real_height < -10.0f)
	{
		teseOut.tex_radio.x = 0;
		teseOut.tex_radio.y = 1 - (upheight - 30.0f)/10.0f;
		teseOut.tex_radio.z =     (upheight - 30.0f)/10.0f;
	}
	else
	{
		teseOut.tex_radio = vec3(0,0,1);
	}

	// Position of the vertex, in worldspace : M * position
	teseOut.Position_worldspace = (M * pos).xyz;
    
    // Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * pos).xyz;
	teseOut.EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = 
    ( V * vec4(LightPosition_worldspace,1)).xyz;
	teseOut.LightDirection_cameraspace = -LightPosition_cameraspace;// + EyeDirection_cameraspace;

	// UV of the vertex. No special space for this one.
	teseOut.UV = texCoord;

	float w  = (textureSize(DiffuseTextureSampler, 0).x - 1);
	float h  = (textureSize(DiffuseTextureSampler, 0).y - 1);	
   	
	float Nx = 0.0f;
	for(int _y = -1 ; _y < 2; ++_y)
	{	
		float left   = compute_height(texture(DiffuseTextureSampler, CheckBoundary(vec2(texCoord.x - 1/w, texCoord.y + _y/h))).rgb * 255.0f, y_scale, y_shift);
		float right  = compute_height(texture(DiffuseTextureSampler, CheckBoundary(vec2(texCoord.x + 1/w, texCoord.y + _y/h))).rgb * 255.0f, y_scale, y_shift);
		Nx += (left - right);
	}
	Nx /= 3.0f;
	float Nz = 0.0f;
	for(int _x = -1 ; _x < 2; ++_x)
	{
		float top = compute_height(texture(DiffuseTextureSampler, CheckBoundary(vec2(texCoord.x + _x/w, texCoord.y + 1/h))).rgb * 255.0f, y_scale, y_shift);
		float bot = compute_height(texture(DiffuseTextureSampler, CheckBoundary(vec2(texCoord.x + _x/w, texCoord.y - 1/h))).rgb * 255.0f, y_scale, y_shift);
		Nz += (top - bot);
	}	
	Nz /= 3.0f;
	//MV3x3 *
	teseOut.Normal_cameraspace =  normalize(vec3(Nx, 0.02, Nz));
}