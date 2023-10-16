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
{ return m_MaterialProperties.textures.albedo; }

std::shared_ptr<Texture2D> Material::GetNormalTexture() const
{ return m_MaterialProperties.textures.normal; }

std::shared_ptr<Texture2D> Material::GetMetallicTexture() const
{ return m_MaterialProperties.textures.metallic; }

std::shared_ptr<Texture2D> Material::GetRoughnessTexture() const
{ return m_MaterialProperties.textures.roughness; }

std::shared_ptr<Texture2D> Material::GetAOTexture() const
{ return m_MaterialProperties.textures.ao; }

void Material::SetAlbedoTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.textures.albedo = texture; }

void Material::SetNormalTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.textures.normal = texture; }

void Material::SetMetallicTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.textures.metallic = texture; }

void Material::SetRoughnessTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.textures.roughness = texture; }

void Material::SetAOTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.textures.ao = texture; }

void Material::SetEmssiveTexture(const std::shared_ptr<Texture2D>& texture)
{ m_MaterialProperties.textures.emissive = texture; }
