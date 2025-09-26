#include "Logger.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace ZPG {
    void Logger::Init() {
        spdlog::set_pattern("%^[%T] [%n]: %v%$");  // $start[time] [name]: message$end
        s_CoreLogger = spdlog::stderr_color_mt("Core");
        s_CoreLogger->set_level(spdlog::level::trace);  // sets min level to log, log everything
        s_ClientLogger = spdlog::stderr_color_mt("Client");
        s_ClientLogger->set_level(spdlog::level::trace);
    }
}
