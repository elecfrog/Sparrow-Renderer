#pragma once

#include "FileDefinitions.h"
#include <spdlog/spdlog.h>
#include <filesystem>
#include <unordered_map>
#include <nfd.h>
#include <stdio.h>
#include <stdlib.h>
#include "Core/Log/LogSystem.h"
#include <ostream>
#include <istream>
#include <iostream>
#include <fstream>
#include "imgui.h"

namespace fs = std::filesystem;
using FilePath = fs::path;
#define Path(x) FileSystem::CheckedPath(x)
#define ShaderPath(x) FileSystem::CheckedPath("./assets/shaders/" x)

class FileSystem
{
public:
    /* copy from Piccolo */
    static std::vector<FilePath> RecursiveGetFiles(const std::string& path);

    static std::string JoinFileRoute(const FilePath& directory, const std::string& fileRoute);
    static void ResolveSlash(std::string& str)
    {
        std::replace(begin(str), end(str), '\\', '/');
    }
    static std::string GetFileExtension(const std::string& path) /* get a specific extension of a file */
    {
	    const FilePath ret = path.c_str();
        return ret.extension().string();
    }
    
    
    static fs::path&& CheckedPath(fs::path&& path) {
        if (fs::is_regular_file(path)) {
            return std::move(path);
        } else {
            std::string description = path.string() + " does not exist or is not a regular file.";
            throw std::runtime_error(description);
        }
    }

	static int64_t GetFileSize(const std::string& path);


    static FileTree& BuildFileTree(const std::string& path);
    static FileTree& GetEngineFileTree();
    static FileTree& GetProjectFileTree();

    static void AddFile(const std::string& directoryPath, const FileMetaInfo& fileInfo);

    void AddSubdirectory(const std::string& parentDirectoryPath, const DirectoryMetaInfo& directoryInfo);

    FileMetaInfo* FindFile(const std::string& filePath);

    DirectoryMetaInfo* FindDirectory(const std::string& directoryPath);

    DirectoryMetaInfo& GetRootDirectory();

    static std::string GetCleanFilename(const std::string& filename);
    /* mount to a virutal Path */
    static void Mount(const std::string& virtualPath, const std::string& realPath);
    static std::string Resolve();						/* get real path back */
    static std::string GetEngineMountPoint();
    static std::string GetProjectMountPoint();
    void CopyEngineFilesToProject();		/* copy engine files to a project */

    static bool FileExists(const std::string& path);
    static bool FolderExists(const std::string& path);

    static uint8_t* ReadFile(const std::string& path);
    static bool ReadFile(const std::string& path, void* buffer, int64_t size = -1);
    static std::string ReadTextFile(const std::string& path);

    static bool WriteFile(const std::string& path, uint8_t* buffer, uint32_t size);
    static bool WriteTextFile(const std::string& path, const std::string& text);

    static std::string GetWorkingDirectory();

    static bool IsRelativePath(const char* path)
    {
        if (!path || path[0] == '/' || path[0] == '\\')
        {
            return false;
        }

        if (strlen(path) >= 2 && isalpha(path[0]) && path[1] == ':')
        {
            return false;
        }

        return true;
    }

    static bool IsAbsolutePath(const char* path)
    {
        if (!path)
        {
            return false;
        }

        return !IsRelativePath(path);
    }

    static const char* GetFileOpenModeString(FileOpenFlags flag)
    {
        if (flag == FileOpenFlags::READ)
        {
            return "rb";
        }
        else if (flag == FileOpenFlags::WRITE)
        {
            return "wb";
        }
        else if (flag == FileOpenFlags::READ_WRITE)
        {
            return "rb+";
        }
        else if (flag == FileOpenFlags::WRITE_READ)
        {
            return "wb+";
        }
        else
        {
            // std:("Invalid open flag");
            return "rb";
        }
    }

    static void DisplayFileSystem(const std::string& path)
    {
        fs::path fs_path = fs::path(path);
        for (const auto& entry : fs::directory_iterator(fs_path))
        {
            auto& entryPath = entry.path();
            if (fs::is_directory(entryPath))
            {
                std::string folderName = entryPath.filename().string();
                spdlog::info("{0} /", folderName);

                // if (ImGui::TreeNode(folderName.c_str()))
                {
                    DisplayFileSystem(entryPath.string());
                    // ImGui::TreePop();
                }
            }
            else
            {
                std::string fileName = entryPath.filename().string();
                // spdlog::info("{0}: ", fileName.c_str());
            	ImGui::Text("%s", fileName.c_str());
            }
        }
    }
    
    static fs::path NativeFileDialog()
    {
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);

        if (result == NFD_OKAY) {
            LogSystem::Info("Success! Path: {0}", outPath);
            fs::path path = outPath;
            free(outPath);
            return path;
        } else if (result == NFD_CANCEL) {
            LogSystem::Info("User pressed cancel.");
        } else {
            LogSystem::Elf_Error(NFD_GetError());
        }
        return fs::path{};
    }

private:
    std::unordered_map<std::string, FileTree> 	m_FileTrees;
};
