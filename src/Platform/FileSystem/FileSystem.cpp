#include "FileSystem.h"


std::vector<FilePath> FileSystem::RecursiveGetFiles(const std::string &path) {
    std::vector<FilePath> files;
    const char *directory = path.c_str();
    for (auto const &directory_entry: fs::recursive_directory_iterator{directory}) {
        if (directory_entry.is_regular_file()) {
            files.emplace_back(directory_entry);
        }
    }
    return files;
}

std::string FileSystem::JoinFileRoute(const FilePath &directory, const std::string &fileRoute) {
    std::string ret = directory.string().append("/").append(fileRoute);
    ResolveSlash(ret);
    return ret;
}

std::string FileSystem::GetCleanFilename(const std::string &filename) {
    return fs::path(filename).filename().replace_extension().string();
}

