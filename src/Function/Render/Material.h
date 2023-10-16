#pragma once

#include "Function/Render/RHI/Shader.h"
#include "Function/Render/RenderDefinitions.h"
#include "Resource/Asset.h"
#include "Function/Render/Texture2D.h"

enum class PBRTextureType
{
    Albedo = 0,
    Normal,
    Metalness,
    Roughness,
    AmbientOcclusion,
    Unknown = 18,
};

struct MaterialProperties
{
    std::string name;

	// Basic Material Properties without PBR
    glm::vec4 diffuseColor;
    glm::vec4 specularColor;
    glm::vec4 ambientColor;
    glm::vec4 emissiveColor;
    glm::vec4 transportColor;
    glm::vec4 refectiveColor;

	enum class WorkFlow
    {
        PBR_WORKFLOW_SEPARATE_TEXTURES,
        PBR_WORKFLOW_METALLIC_ROUGHNESS,
        PBR_WORKFLOW_SPECULAR_GLOSINESS,
    } workflow{};

    /* PBR workflow of Metallic & Roughness */
    struct MetallicRoughnessFactors_
    {
        float baseColorFactor;
        float metallicFactor;
        float roughnessFactor;
        float anisotropyFactor;
    } metallicRoughnessFactors{};

	struct SpecularGlossinessFactors_
    {
        float specularFactor;
        float glossinessFactor;
    } specularGlossinessFactors{};

    struct SheenFactors_
    {
        float sheenFactor;
        float sheenRoughnessFactor;
    } sheenFactors{};

    struct ClearCoatFactors_ 
    {
        float clearcoatFactor;
        float clearcoatRoughnessFactor;
    } clearCoatFactors{};

    struct VolumeFactors_
    {
        float volumeThicknessFactor;
        float volumeAttenuationDistance;
        float attenuationColor;
    } volumeFactors{};

	float transmissionFactor{};
    float emissiveIntensity{};
    float opacity;
    float transparentFactor;
    float bumpScaling;
    float shininess;
    float refelectivity;
    float shininessStrength;
    float refracti;

    struct Textures_
    {
        std::shared_ptr<Texture2D> albedo;
        std::shared_ptr<Texture2D> normal;
        std::shared_ptr<Texture2D> metallic;
        std::shared_ptr<Texture2D> roughness;
        std::shared_ptr<Texture2D> ao;
        std::shared_ptr<Texture2D> emissive;
    } textures{};
};

class Material : public Asset
{
public:
    Material()
		: m_Shader(nullptr), m_MaterialProperties({})
    { }

	explicit Material(MaterialProperties&& prop)
    {
	    m_MaterialProperties = prop;
    }

    // static std::shared_ptr<Material> LoadMaterial(const std::string& filename); // intergrate in model.cpp

    [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
    void SetShader(const std::shared_ptr<Shader>& shader);
    void SetShader(std::shared_ptr<Shader>&& shader);

	[[nodiscard]] MaterialProperties GetMaterialProperites() const;
    void SetMaterialProperites(const MaterialProperties& properties);
    void SetMaterialProperites(MaterialProperties&& properties);

    MaterialProperties::Textures_ GetTextures() const
    {
        return { m_MaterialProperties.textures };
    }
	[[nodiscard]] std::shared_ptr<Texture2D> GetAlbedoTexture() const;
    [[nodiscard]] std::shared_ptr<Texture2D> GetNormalTexture() const;
    [[nodiscard]] std::shared_ptr<Texture2D> GetMetallicTexture() const;
    [[nodiscard]] std::shared_ptr<Texture2D> GetRoughnessTexture() const;
    [[nodiscard]] std::shared_ptr<Texture2D> GetAOTexture() const;

    void SetAlbedoTexture(const std::shared_ptr<Texture2D>& texture);
    void SetNormalTexture(const std::shared_ptr<Texture2D>& texture);
    void SetMetallicTexture(const std::shared_ptr<Texture2D>& texture);
    void SetRoughnessTexture(const std::shared_ptr<Texture2D>& texture);
    void SetAOTexture(const std::shared_ptr<Texture2D>& texture);
    void SetEmssiveTexture(const std::shared_ptr<Texture2D>& texture);

public:
    MaterialProperties m_MaterialProperties;

private:
	std::shared_ptr<Shader> m_Shader;
};
