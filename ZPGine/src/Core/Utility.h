#pragma once

namespace ZPG 
{
    namespace Utility
    {
        std::string ReadFile(const std::string& path);
        std::string GetNameFromPath(const std::string& path);
        f32 GetRandomFloat(f32 min = -1.0f, f32 max = 1.0f);

        template<typename T> concept IsNumeric = std::is_integral_v<T> || std::is_floating_point_v<T>;

        template<IsNumeric T>
        inline T Wrap(T value, T low, T high)
        {
            if (value < low) return high;
            if (value > high) return low;
            return value;
        }
    }
}
