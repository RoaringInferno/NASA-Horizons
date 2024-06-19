#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "filenames.hpp"

class CacheFileName
{
    std::string filename;
public:
    CacheFileName(const std::filesystem::directory_entry& file) :
        filename(file.path().filename().string())
    {};

    std::string getFileName() const {
        return filename;
    };
    std::string getPath() const {
        return CACHE_PATH + getFileName();
    };

};

class CacheMeta
{
    std::vector<CacheFileName> cache_files;
public:
    CacheMeta() : cache_files() {
        for (const auto& entry : std::filesystem::directory_iterator(CACHE_PATH)) {
            if (entry.is_regular_file()) {
                cache_files.push_back(entry);
            }
        }
    }
};