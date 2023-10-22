#include "Material.h"

void Material::SetShader(const std::shared_ptr<Shader>& shader)
{
	m_Shader = shader;
}

void Material::SetShader(std::shared_ptr<Shader>&& shader)
{
	m_Shader = std::move(shader);
}

void Material::SetMaterialProperites(const MaterialProperties& properties)
{
	m_MaterialProperties = properties;
}

void Material::SetMaterialProperites(MaterialProperties&& properties)
{
	m_MaterialProperties = std::move(properties);
}

std::shared_ptr<Shader> Material::GetShader() const
{
	return m_Shader;
}

MaterialProperties Material::GetMaterialProperites() const
{
	return m_MaterialProperties;
}

std::shared_ptr<Texture2D> Material::GetAlbedoTexture() const
{ return m_MaterialProperties.cmTextures.albedo; }

std::shared_ptr<Texture2D> Material::GetNormalTexture() const
{ return m_MaterialProperties.cmTextures.normal; }

std::shared_ptr<Texture2D> Material::GetMetallicTexture() const
{ return m_MaterialProperties.mrTextures.metallic; }

std::shared_ptr<Texture2D> Material::GetRoughnessTexture() const
{ return m_MaterialProperties.mrTextures.roughness; }

std::shared_ptr<Texture2D> Material::GetAOTexture() const
{ return m_MaterialProperties.sgTextures.ao; }

void Material::SetAlbedoTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.cmTextures.albedo = texture; }

void Material::SetNormalTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.cmTextures.normal = texture; }

void Material::SetMetallicTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.mrTextures.metallic = texture; }

void Material::SetRoughnessTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.mrTextures.roughness = texture; }

void Material::SetAOTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.sgTextures.ao = texture; }

void Material::SetEmssiveTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.cmTextures.emissive = texture; }
