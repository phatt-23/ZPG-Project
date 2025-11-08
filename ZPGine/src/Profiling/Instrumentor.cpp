//
// Created by phatt on 11/8/25.
//

#include "Instrumentor.h"

#include "Debug/Asserter.h"

namespace ZPG {

    Instrumentor::Instrumentor(): m_CurrentSession(nullptr), m_ProfileCount(0) {
    }

    Instrumentor::~Instrumentor() {
        if (m_CurrentSession)
            delete m_CurrentSession;
    }

    Instrumentor& Instrumentor::Get() {
        static Instrumentor* instance = new Instrumentor();
        return *instance;
    }

    void Instrumentor::BeginSession(const std::string& name, const std::string& filepath) {
        ZPG_CORE_ASSERT(m_CurrentSession == nullptr, "Session begin called while still in an active session.");
        m_CurrentSession = new InstrumentorSession(name);

        // Write header.
        m_OutputStream.open(filepath);
        m_OutputStream << "{\"otherData\": {},";
        m_OutputStream << "\"traceEvents\": [";
        m_OutputStream.flush();
    }

    void Instrumentor::EndSession() {
        ZPG_CORE_ASSERT(m_CurrentSession != nullptr, "No session to end.");
        delete m_CurrentSession;
        m_CurrentSession = nullptr;

        // Write footer.
        m_OutputStream << "]";
        m_OutputStream << "}";
        m_OutputStream.flush();
        m_OutputStream.close();
        m_ProfileCount = 0;
    }

    void Instrumentor::WriteProfile(const ProfileInfo& profile) {
        if (m_ProfileCount++ > 0)
        {
            m_OutputStream << ",";
        }

        std::string name = profile.Name;
        std::ranges::replace(name, '"', '\"');

        m_OutputStream << "{";
        m_OutputStream << "\"cat\": \"function\",";
        m_OutputStream << "\"name\": \"" << name << "\",";
        m_OutputStream << "\"ts\": \"" << profile.Start << "\",";
        m_OutputStream << "\"dur\": \"" << profile.End - profile.Start << "\",";
        m_OutputStream << "\"ph\": \"X\",";
        m_OutputStream << "\"pid\": \"0\",";
        m_OutputStream << "\"tid\": \"" << profile.ThreadID << "\"";
        m_OutputStream << "}";

        m_OutputStream.flush();
    }


    InstrumentorTimer::InstrumentorTimer(const std::source_location& location)
        : m_Stopped(false), m_StopCallback({})
    {
        m_Name.resize(256);
        int bytes = sprintf(m_Name.data(), "In '%s' at `%s:%d:%d`\n",
                        location.file_name(),
                        location.function_name(),
                        location.line(),
                        location.column());

        m_Name.resize(bytes);
        m_StartTimepoint = std::chrono::steady_clock::now();
    }

    InstrumentorTimer::InstrumentorTimer(std::optional<std::function<void()>>&& stopCallback,
        const std::source_location& location): m_Stopped(false), m_StopCallback(stopCallback) {

        m_Name.resize(256);
        int bytes = sprintf(m_Name.data(), "In '%s' at `%s:%d:%d`\n",
                        location.file_name(),
                        location.function_name(),
                        location.line(),
                        location.column());

        m_Name.resize(bytes);

        m_StartTimepoint = std::chrono::steady_clock::now();
    }


    InstrumentorTimer::InstrumentorTimer(std::string&& name)
        : m_Name(name), m_Stopped(false), m_StopCallback({})
    {
        m_StartTimepoint = std::chrono::steady_clock::now();
    }

    InstrumentorTimer::InstrumentorTimer(std::string&& name, std::optional<std::function<void()>>&& stopCallback): m_Name(name), m_Stopped(false), m_StopCallback(stopCallback) {
        m_StartTimepoint = std::chrono::steady_clock::now();
    }

    InstrumentorTimer::~InstrumentorTimer() {
        if (!m_Stopped)
            Stop();
    }

    void InstrumentorTimer::Stop() {
        auto endTimepoint = std::chrono::steady_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimepoint).time_since_epoch().count();

        std::uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
        Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

        if (m_StopCallback)
            m_StopCallback.value()();

        m_Stopped = true;
    }
} // ZPG