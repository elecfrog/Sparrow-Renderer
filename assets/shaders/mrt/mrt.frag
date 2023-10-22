#version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 PositionColor;
layout (location = 3) out vec4 NormalColor;
layout (location = 4) out vec4 AmbientColor;

// Input Data from Vertex Shader
in vec2 fragTexCoords;
in vec3 vert_worldPos;
in mat3 TBN;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

// uniform uint WORKFLOW;

const float PI = 3.14159265359;
const vec3 dielectric_constant = vec3(0.04);

const uint PBR_MR_WORKFLOW = 0;
const uint PBR_SG_WORKFLOW = 1;

// Textures
struct Material
{
    // PBR Workflow Option
    uint WORKFLOW;

    sampler2D albedoTexture;
    sampler2D normalTexture;
    sampler2D metallicRoughnessTexture;
    sampler2D emissiveTexture;
    sampler2D aoTex;
    sampler2D sgTex;

    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
    float IOR;
    int  illum;
};
uniform Material material;

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Light lit;

// normal mapping
vec3 calulateNormals()
{
    vec3 n = texture(material.normalTexture, vec2(fragTexCoords.x, fragTexCoords.y)).rgb * 2.0f - 1.0f;
    n = TBN * n;// convert to [0, 1]

    return n;
}

float shininess(float roughness)
{
    return 2 / (pow(roughness, 4) + 0.0001) - 2;
}

// Frensel_Term
vec3 Frensel_Term(vec3 F0, vec3 albedo, float metallic, float HdotV)
{
    return F0 + (1 - F0) * pow((1.0 - HdotV), 5.0);
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2
float Distribution_ndfGGX(float cosLh, float roughness)
{
    float alpha   = roughness * roughness;
    float alphaSq = alpha * alpha;

    float  denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

float GaSchlickGGX_Item(float ndoth, float ndotv, float vdoth, float ndotl)
{
    return min(1, min(2 * (ndoth * ndotv) / vdoth, 2 * (ndoth * ndotl)  / vdoth));
}

vec3 Lambertian(vec3 F, float metallic)
{
    return (1 - F) * (1 - metallic);
}

// implement the light square-law falloff, where I setup the radius of the light source to be 2.0
float falloff()
{
    vec3 L = normalize(lightPos.xyz - vert_worldPos);

    float dist = length(L);

    L = normalize(L);

    float atten = (2.) / (pow(dist, 2.0) + 1.0);

    return atten;
}


vec3 point_lighting()
{
    // Material properties
    vec3 albedoColor  = texture(material.albedoTexture, vec2(fragTexCoords.x, fragTexCoords.y)).rgb;

    // In gltf, metallic and roughness are stored in the same texture with R and G channels respectively
    float metallic  = texture(material.metallicRoughnessTexture, fragTexCoords).r;
    float roughness = texture(material.metallicRoughnessTexture, fragTexCoords).g;
    float ao = texture(material.metallicRoughnessTexture, fragTexCoords).b;

    vec3 emissive = texture(material.emissiveTexture, fragTexCoords).rgb;

    // normal read from texture sampler, which is in tangent space, and which in [-1, 1], and we need to convert it to [0, 1]
    vec3 n = calulateNormals();
    // n = normalize(TBN * n);		 // convert to world space


    float alpha_p = shininess(roughness);

    //	vec3 MaterialSpecularColor = vec3(1,1,1);

    // view direction, point to the camera
    vec3 v = normalize(viewPos.xyz - vert_worldPos);
    vec3 l = normalize(lightPos.xyz - vert_worldPos);
    vec3 h = normalize(v + l);

    float HdotV = dot(h, v);
    float NdotH = max(dot(n, h), 0.0);
    float NdotV = max(dot(n, v), 0.0);
    float NdotL = max(dot(n, l), 0.0);
    float VdotH = dot(v, h);

    vec3  F0 = (1 - metallic) * dielectric_constant + metallic * albedoColor;

    // F = k_s in the reflection equation
    vec3  F = Frensel_Term(F0, albedoColor, metallic, HdotV);


    float D = Distribution_ndfGGX(NdotH, roughness);

    float G = GaSchlickGGX_Item(NdotH, NdotV, VdotH, NdotL);


    vec3 specularBRDF = (F * D * G) / max(0.000001, 4.0 * NdotL * NdotV);


    vec3 diffuseBRDF = Lambertian(F, metallic) * (albedoColor/ PI);

    vec3 lightContribution = (specularBRDF + diffuseBRDF) * vec3(lightColor) * NdotL * falloff();
    vec3 ambientContribution = albedoColor;

    return lightContribution + ambientContribution * ao + emissive;
}

vec3 calulate_MR_BRDF(float HdotV, float NdotH, float NdotV, float NdotL, float VdotH, vec3 albedoColor)
{
    // [ref](https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html section 3.9.2)
    // In gltf, metallic and roughness are stored in the same texture with R and G channels respectively
    float roughness = texture(material.metallicRoughnessTexture, fragTexCoords).g;
    float metallic  = texture(material.metallicRoughnessTexture, fragTexCoords).b;
    float ao = texture(material.metallicRoughnessTexture, fragTexCoords).r;

    float alpha_p = shininess(roughness);
    
    vec3  F0 = (1 - metallic) * dielectric_constant + metallic * albedoColor;

    // F = k_s in the reflection equation
    vec3  F = Frensel_Term(F0, albedoColor, metallic, HdotV);

    float D = Distribution_ndfGGX(NdotH, roughness);

    float G = GaSchlickGGX_Item(NdotH, NdotV, VdotH, NdotL);
    
    vec3 specularBRDF = (F * D * G) / max(0.000001, 4.0 * NdotL * NdotV);

    vec3 diffuseBRDF = Lambertian(F, metallic) * (albedoColor/ PI);
    
    return specularBRDF + diffuseBRDF;
}

vec3 calulate_SG_BRDF(float HdotV, float NdotH, float NdotV, float NdotL, float VdotH, vec3 albedoColor)
{
    // [ref](https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html section 3.9.2)
    // In gltf, metallic and roughness are stored in the same texture with R and G channels respectively
    vec3 specularGlossiness = texture(material.sgTex, fragTexCoords).rgb;
    float roughness  = 0.f;
    float ao = texture(material.aoTex, fragTexCoords).r;

    float alpha_p = shininess(roughness);

    vec3 F0 = specularGlossiness;

    // F = k_s in the reflection equation
    vec3  F = Frensel_Term(F0, albedoColor, roughness, HdotV);

    float D = Distribution_ndfGGX(NdotH, roughness);

    float G = GaSchlickGGX_Item(NdotH, NdotV, VdotH, NdotL);

    vec3 specularBRDF = (F * D * G) / max(0.000001, 4.0 * NdotL * NdotV);

    vec3 diffuseBRDF = Lambertian(F, roughness) * (albedoColor/ PI);
    
//    return specularBRDF + diffuseBRDF;
    return vec3(specularGlossiness);
}

void main()
{
    // Material properties
    vec3 albedoColor  = texture(material.albedoTexture, vec2(fragTexCoords.x, fragTexCoords.y)).rgb;

    // normal read from texture sampler, which is in tangent space, and which in [-1, 1], and we need to convert it to [0, 1]
    vec3 n = calulateNormals();
    // view direction, point to the camera
    vec3 v = normalize(viewPos.xyz - vert_worldPos);
    vec3 l = normalize(lightPos.xyz - vert_worldPos);
    vec3 h = normalize(v + l);

    float HdotV = dot(h, v);
    float NdotH = max(dot(n, h), 0.0);
    float NdotV = max(dot(n, v), 0.0);
    float NdotL = max(dot(n, l), 0.0);
    float VdotH = dot(v, h);
    
    // BRDF
    vec3 LightBRDF = vec3(0.f);
    if(material.WORKFLOW == PBR_MR_WORKFLOW)
    {
        LightBRDF = calulate_MR_BRDF(HdotV, NdotH, NdotV, NdotL, VdotH, albedoColor) * vec3(lightColor) * NdotL * falloff();
    }
    else if(material.WORKFLOW == PBR_SG_WORKFLOW)
    {
        LightBRDF = calulate_SG_BRDF(HdotV, NdotH, NdotV, NdotL, VdotH, albedoColor) * vec3(lightColor) * NdotL * falloff();
    }
    
    vec3 ambientContribution = albedoColor;

    vec3 emissive = texture(material.emissiveTexture, fragTexCoords).rgb;


    vec3 oColor = LightBRDF + emissive;//+ ambientContribution * ao ;
    //	vec3 n = calulateNormals();
    FragColor = vec4(oColor, 1.0f);
    BrightColor = vec4(emissive, 1.0f);
    PositionColor = vec4(vert_worldPos, 1.0f);
    NormalColor = vec4(n, 1.0f);
//    AmbientColor  = vec4(vec3(0.f), 1.0f);
    AmbientColor  = vec4(albedoColor, 1.0f);
}