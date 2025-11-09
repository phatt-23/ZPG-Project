//
// Created by phatt on 11/8/25.
//

#ifndef WORKSPACE_INSTRUMENTOR_H
#define WORKSPACE_INSTRUMENTOR_H

namespace ZPG {

    struct ProfileInfo
    {
        std::string Name;
        long long Start;
        long long End;
        std::uint32_t ThreadID;
    };

    struct InstrumentorSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    private:
        Instrumentor();

    public:
        ~Instrumentor();
        static Instrumentor& Get();
        void BeginSession(const std::string& name, const std::string& filepath = "ProfileResult.json");
        void EndSession();
        void WriteProfile(const ProfileInfo& profile);

    private:
        InstrumentorSession* m_CurrentSession;
        unsigned int m_ProfileCount;
        std::ofstream m_OutputStream;
    };

    class InstrumentorTimer
    {
    public:
        InstrumentorTimer(const std::source_location& location = std::source_location::current());
        InstrumentorTimer(std::optional<std::function<void(void)>>&& stopCallback, const std::source_location& location = std::source_location::current());
        InstrumentorTimer(std::string&& name);
        InstrumentorTimer(std::string&& name, std::optional<std::function<void(void)>>&& stopCallback);

        ~InstrumentorTimer();

        void Stop();

    private:
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        std::string m_Name;
        bool m_Stopped;
        std::optional<std::function<void(void)>> m_StopCallback;
    };

} // ZPG

#define ZPG_CONCATENATE_DETAIL(x, y)         x##y
#define ZPG_CONCATENATE(x, y)                ZPG_CONCATENATE_DETAIL(x, y)
#define ZPG_SCOPE_UNIQUE_VAR_NAME(base)      ZPG_CONCATENATE(base, __COUNTER__)

#if ZPG_ENABLE_PROFILING
    #define ZPG_PROFILE_BEGIN_SESSION(name, filepath)    ::ZPG::Instrumentor::Get().BeginSession(name, filepath)
    #define ZPG_PROFILE_END_SESSION()                    ::ZPG::Instrumentor::Get().EndSession()
    #define ZPG_PROFILE_SCOPE(name)                      ::ZPG::InstrumentorTimer ZPG_SCOPE_UNIQUE_VAR_NAME(__timer)(name)
    #define ZPG_PROFILE_FUNCTION()                       ZPG_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
    #define ZPG_PROFILE_BEGIN_SESSION(name, filepath)
    #define ZPG_PROFILE_END_SESSION()
    #define ZPG_PROFILE_SCOPE(name)
    #define ZPG_PROFILE_FUNCTION()
#endif

#endif //WORKSPACE_INSTRUMENTOR_H