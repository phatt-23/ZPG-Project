//
// Created by phatt on 9/22/25.
//

#ifndef ASSERTER_H
#define ASSERTER_H

#include <string>
#include <source_location>
#include <format>
#include "Logger.h"

namespace ZPG {

class Asserter {
public:
    // with formated message
    template<typename ...Args>
    static bool CoreAssert(
        std::source_location&& location, 
        std::string&& expression,
        bool result, 
        const std::string& format, 
        Args&&... args
    ) {
        if (result) 
            return true;
        std::string msg = std::vformat(format, std::make_format_args(args...));
        ZPG_CORE_ERROR("Assertion '{}' Failed: {}:{}:{}:{}: {}", 
                       expression,
                       location.file_name(), 
                       location.line(), 
                       location.column(),
                       location.function_name(),
                       msg);
        return false;
    }

    template<typename ...Args>
    static bool ClientAssert(
        std::source_location&& location, 
        std::string&& expression,
        bool result, 
        const std::string& format, 
        Args&&... args
    ) {
        if (result) 
            return true;
        std::string msg = std::vformat(format, std::make_format_args(args...));
        ZPG_ERROR("Assertion '{}' Failed: {}:{}:{}:{}: {}", 
                       expression,
                       location.file_name(), 
                       location.line(), 
                       location.column(),
                       location.function_name(),
                       msg);
        return false;
    }

    // without message
    template<typename ...Args>
    static bool CoreAssert(
        std::source_location&& location, 
        std::string&& expression,
        bool result
    ) {
        if (result) 
            return true;
        ZPG_ERROR("Assertion '{}' Failed: {}:{}:{}:{}", 
                       expression,
                       location.file_name(), 
                       location.line(), 
                       location.column(),
                       location.function_name());
        return false;
    }

    template<typename ...Args>
    static bool ClientAssert(
        std::source_location&& location, 
        std::string&& expression,
        bool result
    ) {
        if (result) 
            return true;
        ZPG_ERROR("Assertion '{}' Failed: {}:{}:{}:{}", 
                       expression,
                       location.file_name(), 
                       location.line(), 
                       location.column(),
                       location.function_name());
        return false;
    }


};

}

#define ZPG_ENABLE_ASSERTS
#ifdef ZPG_ENABLE_ASSERTS
    #define ZPG_CORE_ASSERT(condition, ...) \
        { if (!Asserter::CoreAssert(std::source_location::current(), \
                                    #condition, \
                                    condition \
                                    __VA_OPT__(, __VA_ARGS__))) \
            __builtin_trap(); \
        }

    #define ZPG_ASSERT(condition, ...) \
        { if (!Asserter::ClientAssert(std::source_location::current(), \
                                    #condition, \
                                    condition \
                                    __VA_OPT__(, __VA_ARGS__))) \
            __builtin_trap(); \
        }
    #define ZPG_UNREACHABLE(...) \
        { auto loc = std::source_location::current(); \
            ZPG_CORE_CRITICAL("Unreachable code {}:{}:{}:{}", loc.file_name(), loc.line(), loc.column(), loc.function_name()); \
            __VA_OPT__(ZPG_CORE_CRITICAL(__VA_ARGS__);) \
            __builtin_trap(); \
        }

    #define ZPG_NOT_IMPL(...) \
        { auto loc = std::source_location::current(); \
            ZPG_CORE_CRITICAL("Not implemented {}:{}:{}:{}", loc.file_name(), loc.line(), loc.column(), loc.function_name()); \
            __VA_OPT__(ZPG_CORE_CRITICAL(__VA_ARGS__);) \
            __builtin_trap(); \
        }
#else
    #define ZPG_CORE_ASSERT(condition, format, ...)
    #define ZPG_ASSERT(condition, format, ...)
#endif

#endif //ASSERTER_H
