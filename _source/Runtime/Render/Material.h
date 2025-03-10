#pragma once

#include <Core/Core.h>
#include "RHI/Shader.h"
#include "Texture2D.h"

namespace Sparrow
{
    enum class PBRTextureType
    {
        Albedo = 0,
        Normal,
        Metalness,
        Roughness,
        AmbientOcclusion,
        Unknown = 18,
    };

    struct MaterialTextures
    {
        std::shared_ptr<Texture2D> albedo;
        std::shared_ptr<Texture2D> normal;
        std::shared_ptr<Texture2D> metallic;
        std::shared_ptr<Texture2D> roughness;
        std::shared_ptr<Texture2D> ao;
        std::shared_ptr<Texture2D> emissive;
    };

    struct MaterialProperties
    {
        std::string name;

        // Basic Material Properties without PBR
        Vector4f diffuseColor;
        Vector4f specularColor;
        Vector4f ambientColor;
        Vector4f emissiveColor;
        Vector4f transportColor;
        Vector4f refectiveColor;

        enum class WorkFlow
        {
            PBR_WORKFLOW_SEPARATE_TEXTURES,
            PBR_WORKFLOW_METALLIC_ROUGHNESS,
            PBR_WORKFLOW_SPECULAR_GLOSINESS,
        } workflow{};

        /* PBR workflow of Metallic & Roughness */
        struct MetallicRoughnessFactors_
        {
            Float baseColorFactor;
            Float metallicFactor;
            Float roughnessFactor;
            Float anisotropyFactor;
        } metallicRoughnessFactors{};

        struct SpecularGlossinessFactors_
        {
            Float specularFactor;
            Float glossinessFactor;
        } specularGlossinessFactors{};

        struct SheenFactors_
        {
            Float sheenFactor;
            Float sheenRoughnessFactor;
        } sheenFactors{};

        struct ClearCoatFactors_
        {
            Float clearcoatFactor;
            Float clearcoatRoughnessFactor;
        } clearCoatFactors{};

        struct VolumeFactors_
        {
            Float volumeThicknessFactor;
            Float volumeAttenuationDistance;
            Float attenuationColor;
        } volumeFactors{};

        Float transmissionFactor{};
        Float emissiveIntensity{};
        Float opacity;
        Float transparentFactor;
        Float bumpScaling;
        Float shininess;
        Float refelectivity;
        Float shininessStrength;
        Float refracti;

        MaterialTextures textures{};
    };

    class Material : public Asset
    {
    public:
        Material()
            : m_Shader(nullptr), m_MaterialProperties({})
        {
        }

        explicit Material(MaterialProperties&& prop)
        {
            m_MaterialProperties = prop;
        }

        [[nodiscard]] std::shared_ptr<Shader> GetShader() const;
        void SetShader(const std::shared_ptr<Shader>& shader);
        void SetShader(std::shared_ptr<Shader>&& shader);

        [[nodiscard]] MaterialProperties GetMaterialProperites() const;
        void SetMaterialProperites(const MaterialProperties& properties);
        void SetMaterialProperites(MaterialProperties&& properties);

        MaterialTextures GetTextures() const
        {
            return {m_MaterialProperties.textures};
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
}
