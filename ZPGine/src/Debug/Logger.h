//
// Created by phatt on 9/22/25.
//

#pragma once

namespace ZPG {

class Logger {
    using LoggerRef = ref<spdlog::logger>;
public:
    static void Init();

    static LoggerRef& GetCoreLogger() { return s_CoreLogger; }
    static LoggerRef& GetClientLogger() { return s_ClientLogger; }
private:
    inline static LoggerRef s_CoreLogger = nullptr;
    inline static LoggerRef s_ClientLogger = nullptr;
};

}
#ifdef ZPG_ENABLE_LOGGING
    #define ZPG_CORE_TRACE(...) ::ZPG::Logger::GetCoreLogger()->trace(__VA_ARGS__)
    #define ZPG_CORE_DEBUG(...) ::ZPG::Logger::GetCoreLogger()->debug(__VA_ARGS__)
    #define ZPG_CORE_INFO(...) ::ZPG::Logger::GetCoreLogger()->info(__VA_ARGS__)
    #define ZPG_CORE_WARN(...) ::ZPG::Logger::GetCoreLogger()->warn(__VA_ARGS__)
    #define ZPG_CORE_ERROR(...) ::ZPG::Logger::GetCoreLogger()->error(__VA_ARGS__)
    #define ZPG_CORE_CRITICAL(...) ::ZPG::Logger::GetCoreLogger()->critical(__VA_ARGS__)
    
    #define ZPG_TRACE(...) ::ZPG::Logger::GetClientLogger()->trace(__VA_ARGS__)
    #define ZPG_DEBUG(...) ::ZPG::Logger::GetClientLogger()->debug(__VA_ARGS__)
    #define ZPG_INFO(...) ::ZPG::Logger::GetClientLogger()->info(__VA_ARGS__)
    #define ZPG_WARN(...) ::ZPG::Logger::GetClientLogger()->warn(__VA_ARGS__)
    #define ZPG_ERROR(...) ::ZPG::Logger::GetClientLogger()->error(__VA_ARGS__)
    #define ZPG_CRITICAL(...) ::ZPG::Logger::GetClientLogger()->critical(__VA_ARGS__)
#else
    #define ZPG_CORE_TRACE(...)
    #define ZPG_CORE_DEBUG(...)
    #define ZPG_CORE_INFO(...)
    #define ZPG_CORE_WARN(...)
    #define ZPG_CORE_ERROR(...)
    #define ZPG_CORE_CRITICAL(...)
    
    #define ZPG_TRACE(...)
    #define ZPG_DEBUG(...)
    #define ZPG_INFO(...) 
    #define ZPG_WARN(...)
    #define ZPG_ERROR(...)
    #define ZPG_CRITICAL(...)
#endif
