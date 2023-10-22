#version 450 core

// Ouput Data, the only is color
out vec4 color;

in VERTEX_OUT
{
    vec3 view_Normal;
    vec2 fragTexCoords;

    vec3 world_Position;
    vec3 world_Normals;
} vertex_in;

// Lighting Variables
struct LIGHT
{
    vec3 position;
    vec3 specularColor;
    vec3 diffuseColor;
    vec3 ambientColor;
};
uniform LIGHT light;

struct MATERIAL
{
    vec4 baseColor;
};
uniform MATERIAL material;


uniform vec3 viewPos;

// Texturess
uniform sampler2D tex_Diffuse;

const float PI = 3.14159265359;
const float DIELECTRIC = 0.04;

uniform float u_Roughness;


// Normal Distribution Function using GGX Distribution
float D_GGX(float NdotH, float roughness)
{
    float a2 = roughness * roughness;
    float f = (NdotH * NdotH) * (a2 - 1.f) + 1.f;
    return a2 / (PI * f * f);
}

// Geometry Term: Geometry masking/shadowing due to microfacets
float GGX(float NdotV, float k)
{
    return NdotV / (NdotV * (1.f-k) + k);
}

float G_Smith(float NdotV, float NdotL, float roughness)
{
    float k = pow(roughness + 1.f, 2.f) /8.f;
    return GGX(NdotL, k) * GGX(NdotV, k);
}

// Fresnerl term with scalar optimization
vec3 F_Schlick(float VdotH, vec3 f0)
{
    float f = pow(1.f - VdotH, 5.f);
    return f0 + (1.f - f0) * f;
}

vec3 Lambertian(vec3 F, float roughness)
{
    return (1 - F) * (1 - roughness);
}

//// implement the light square-law falloff, where I setup the radius of the light source to be 2.0
//float falloff(vec3 L)
//{
//    vec3 L = normalize(lightPos.xyz - vert_worldPos);
//
//    float dist = length(L);
//
//    L = normalize(L);
//
//    float atten = (2.) / (pow(dist, 2.0) + 1.0);
//
//    return atten;
//}

void main()
{
    vec3 N = normalize(vertex_in.world_Normals);

    vec3 L = normalize(light.position - vertex_in.world_Position);

    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * light.diffuseColor;

    // Camera Direction
    vec3 V = normalize(viewPos - vertex_in.world_Position);
    // Haf Vector
    vec3 H = normalize(L + V);

    //	vec3 MaterialSpecularColor = vec3(1,1,1);

    //	// view direction, point to the camera
    //	vec3 l = normalize(lightPos.xyz - vert_worldPos);
    //	vec3 H = normalize(V + l);

    //	float HdotV = dot(h, V);
    float NdotH = max(dot(N, H), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    //	vec3  F0 = (1 - metallic) * dielectric_constant + metallic * albedoColor;
    //
    //	// F = k_s in the reflection equation
    //	vec3  F = Frensel_Term(F0, albedoColor, metallic, HdotV);

    //	float roughness = 0.5f;
    // D -- Normal Distribution Function
    float D = D_GGX(NdotH, u_Roughness);

    // G -- Geometry masking/shadowing due to microfacets
    float G = G_Smith(NdotV, NdotL, u_Roughness);

    vec3 F0 = (1 - u_Roughness) * vec3(DIELECTRIC) + u_Roughness * vec3(material.baseColor);

    // F -- Fresnerl term with scalar optimization
    vec3 F = F_Schlick(VdotH, F0);


    vec3 SpecularBRDF = (D * G  * F) / max(0.000001, 4.0 * NdotL * NdotV);

    vec3 DiffuseBRDF = Lambertian(F, u_Roughness) * (vec3(material.baseColor)/ PI);

    vec3 LightContribution = (SpecularBRDF + DiffuseBRDF) * vec3(1.f) * NdotL /** falloff()*/;
    vec3 AmbientContribution = vec3(0.f) * 0.04;

    vec3 ResutltColor = LightContribution + AmbientContribution;
    // LightContribution + AmbientContribution;
    color = vec4(ResutltColor, 1.0);
}