#pragma once
#include <filesystem>
class Resource
{
public:
	virtual void load(const std::filesystem::path& filePath) = 0;
};