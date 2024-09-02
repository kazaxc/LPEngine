#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "../util/DateTime.h"
#include "Defines.h"

namespace LPEngine
{
    enum class LPENGINE_API LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    class LPENGINE_API Logger
    {
    public:
        static void Init();
        static void Terminate();

        template<typename... Args>
        static void Log(LogLevel level, std::string const& message, Args... args)
        {
            std::ostringstream formattedMessage;
            formattedMessage << "[" << DateTime::DateTime() << "] [" << LogLevelToString(level) << "] " << message << " ";
            ((formattedMessage << args << " "), ...);
            std::string outputMessage = formattedMessage.str();

            std::cout << outputMessage << std::endl;
            (*m_stream) << outputMessage << std::endl;
        }

    private:
        static std::ofstream* m_stream;
        static std::string LogLevelToString(LogLevel level);
    };
}