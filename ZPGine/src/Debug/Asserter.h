//
// Created by phatt on 9/22/25.
//

#ifndef ASSERTER_H
#define ASSERTER_H

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

#define ZPG_PRINT_STACKTRACE() \
    {                                                   \
        std::stringstream ss;                           \
        ss << boost::stacktrace::stacktrace();          \
        ZPG_CORE_ERROR("{}", ss.str().c_str());      \
    }

#ifdef ZPG_ENABLE_ASSERTS
    #define ZPG_CORE_ASSERT(condition, ...) \
        { \
            if (!Asserter::CoreAssert(std::source_location::current(), \
                                    #condition, \
                                    condition \
                                    __VA_OPT__(, __VA_ARGS__))) { \
                ZPG_PRINT_STACKTRACE(); \
                __builtin_trap(); \
            } \
        }

    #define ZPG_ASSERT(condition, ...) \
        { \
            if (!Asserter::ClientAssert(std::source_location::current(), \
                                    #condition, \
                                    condition \
                                    __VA_OPT__(, __VA_ARGS__))) { \
                ZPG_PRINT_STACKTRACE(); \
                __builtin_trap(); \
            } \
        }
    #define ZPG_UNREACHABLE(...) \
        { \
            auto loc = std::source_location::current(); \
            ZPG_CORE_CRITICAL("Unreachable code {}:{}:{}:{}", loc.file_name(), loc.line(), loc.column(), loc.function_name()); \
            __VA_OPT__(ZPG_CORE_CRITICAL(__VA_ARGS__);) \
            ZPG_PRINT_STACKTRACE(); \
            __builtin_trap(); \
        }

    #define ZPG_NOT_IMPL(...) \
        { \
            auto loc = std::source_location::current(); \
            ZPG_CORE_CRITICAL("Not implemented {}:{}:{}:{}", loc.file_name(), loc.line(), loc.column(), loc.function_name()); \
            __VA_OPT__(ZPG_CORE_CRITICAL(__VA_ARGS__);) \
            ZPG_PRINT_STACKTRACE(); \
            __builtin_trap(); \
        }
#else
    #define ZPG_CORE_ASSERT(condition, ...)
    #define ZPG_ASSERT(condition, ...)
    #define ZPG_UNREACHABLE(...) 
    #define ZPG_NOT_IMPL(...)
#endif



#endif //ASSERTER_H
