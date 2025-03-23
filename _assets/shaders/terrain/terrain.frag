#version 450 core

// Interpolated values from the vertex shaders
in TESE_DATA
{
	vec2 UV;
	vec3 Position_worldspace;
	vec3 EyeDirection_cameraspace;
	vec3 LightDirection_cameraspace;
	vec3 Normal_cameraspace;
	vec3 tex_radio;
}dataIn;

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D DiffuseTextureSampler;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform vec3 LightPosition_worldspace;

struct Matrial
{
	sampler2D rock; 
	sampler2D grass; 
	sampler2D snow; 

	sampler2D rock_s; 
	sampler2D grass_s; 
	sampler2D snow_s; 
};
uniform Matrial rt;


void main(){

	// Some properties
	// should put them as uniforms
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 1.0;
	float shininess = 1;

	// Material properties
	// Tiling Method Come From: https://stackoverflow.com/questions/6473321/tiling-texture-in-shader
	vec2 tiling = textureSize(DiffuseTextureSampler, 0) / textureSize(rt.grass, 0);
	vec2 normTexUV = tiling * vec2(dataIn.UV.x,dataIn.UV.y);
	vec3 MaterialDiffuseColor = dataIn.tex_radio.x * texture(rt.grass,normTexUV).rgb
							+ dataIn.tex_radio.y * texture(rt.rock,normTexUV).rgb
							+ dataIn.tex_radio.z * texture(rt.snow,normTexUV).rgb;
	vec3 MaterialSpecularColor = dataIn.tex_radio.x * texture(rt.grass_s,normTexUV).rgb
							+ dataIn.tex_radio.y * texture(rt.rock_s,normTexUV).rgb
							+ dataIn.tex_radio.z * texture(rt.snow_s,normTexUV).rgb; 

	vec3 MaterialAmbientColor = vec3(0.2,0.2,0.2) * MaterialDiffuseColor;
	// vec3 MaterialSpecularColor = vec3(1,1,1);

	// Distance to the light
	//float distance = length( LightPosition_worldspace - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = dataIn.Normal_cameraspace;
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(dataIn.LightDirection_cameraspace);
	vec3 e = normalize(dataIn.EyeDirection_cameraspace);

	// Diffuse
	float cosTheta = clamp( dot( n,l ), 0,1 );
	vec3 diffuse = MaterialDiffuseColor * LightColor * LightPower * cosTheta ;// (distance*distance) ;
	
	// Specular
	// Eye vector (towards the camera)
	vec3 E = normalize(dataIn.EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 B = normalize(l + e);

	float cosB = clamp(dot(n,B),0,1);
	cosB = clamp(pow(cosB,shininess),0,1);
	cosB = cosB * cosTheta * (shininess+2)/(2*radians(180.0f));
	vec3 specular = MaterialSpecularColor *LightPower*cosB;//(distance*distance);
	
	// color = n;
	color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		diffuse +
		// Specular : reflective highlight, like a mirror
		specular;

}