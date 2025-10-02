#pragma once

#include "Core/Core.h"
#include "Debug/Logger.h"
#include "Debug/Asserter.h"

namespace ZPG {

class Utility {
public:
    static std::string ReadFile(const std::string& path) {
        ZPG_CORE_DEBUG("Reading file from CWD: {}", std::filesystem::current_path().c_str());

        std::ifstream f(path, std::ios::binary); 
        ZPG_CORE_ASSERT(f.is_open(), "File {} failed to open", path);

        f.seekg(0, std::ios::end);
        std::string content;
        u32 fileSize = f.tellg();

        ZPG_CORE_INFO("File size: {}", fileSize);
        ZPG_CORE_ASSERT(fileSize > 0, "File {} is empty", path);

        content.resize(fileSize);
        f.seekg(0, std::ios::beg);
        f.read(content.data(), content.size());
        f.close();

        return std::move(content);
    }

    static std::string GetNameFromPath(const std::string& path) {
        size_t lastSlash = path.find_last_of("/\\");  // linux and windows slash
        size_t lastDot = path.rfind(".");
        auto nameBegin = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto nameEnd = lastDot == std::string::npos ? path.size() : lastDot;
        return path.substr(nameBegin, nameEnd - nameBegin);
    }
};

}
