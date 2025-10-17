#pragma once

namespace ZPG {

class Utility {
public:
    static std::string ReadFile(const std::string& path);
    static std::string GetNameFromPath(const std::string& path);
    static f32 GetRandomFloat(f32 min = -1.0f, f32 max = 1.0f);
};

}
