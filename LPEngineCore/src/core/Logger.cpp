#include <filesystem>

#include "../../include/core/Logger.h"

namespace LPEngine
{
    std::ofstream* Logger::m_stream;

    void Logger::Init()
    {
        // Check if the logs directory exists, if not create it
        if (!std::filesystem::exists("logs"))
        {
            std::filesystem::create_directory("logs");
        }

        m_stream = new std::ofstream();

        // Check if the stream is already open, if not open it
        if (!m_stream->is_open())
        {
            m_stream->open("logs/" + DateTime::DateTime(true) + ".log", std::ios::out | std::ios::app);
        }
        else
        {
            Log(LogLevel::ERROR, "Logger already initialised");
        }
    }

    void Logger::Terminate()
    {
        Log(LogLevel::INFO, "Terminating Logger");
        m_stream->close();
        delete m_stream;
    }

    std::string Logger::LogLevelToString(LogLevel level)
    {
        switch (level)
        {
        using enum LPEngine::LogLevel;
        case TRACE:
            return "TRACE";
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }
}