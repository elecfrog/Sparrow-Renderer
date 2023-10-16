#pragma once

#include <map>
#include <string>
#include <vector>

enum class FileOpenFlags
{
    READ,
    WRITE,
    READ_WRITE,
    WRITE_READ
};

// FileExtensions
enum class FileExtensions
{
	GLTF,
	FBX,
	PNG,
	BVH,
	// ...
};

enum class FileTreeType
{
	ENGINE,
	PROJECT,
    SHADER,
    SCRIPT,
    DEFAULT
};

const std::map<FileExtensions, std::string> k_FileExtensionsMap =
{
    {FileExtensions::GLTF, ".gltf"},
    {FileExtensions::FBX, ".fbx"},
    {FileExtensions::PNG, ".png"},
    {FileExtensions::BVH, ".bvh"},
    // ...
};

struct FileMetaInfo
{
    std::string name{};
    std::string fullPath{};
};

struct DirectoryMetaInfo
{
    std::string name{};
    std::string fullPath{};
    std::vector<DirectoryMetaInfo> 	subDirectories{};
    std::vector<FileMetaInfo> 		files{};
};

struct FileTree
{
    FileTreeType      type{FileTreeType::DEFAULT};               /* type of the file tree*/
    DirectoryMetaInfo mountPoint{}; 		/* virutal path dir */
    DirectoryMetaInfo resolvePoint{};		/* real path dir */
};