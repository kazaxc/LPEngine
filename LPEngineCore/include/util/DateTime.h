#pragma once

#include <string>
#include <chrono>

#include "../core/Defines.h"

namespace LPEngine
{
    namespace DateTime
    {
        // Returns a string with the date and time in the current timezone - (YYYY-MM-DD HH:MM:SS)
        // Optionally, the string can be formatted to be used as a filename - (YYYY-MM-DD_HH-MM-SS)
        static std::string DateTime(bool filename = false)
        {
            auto now = std::chrono::system_clock::now();
            auto time_zone = std::chrono::current_zone();
            auto zt = std::chrono::zoned_time{ time_zone, now };
            auto floored_time = std::chrono::floor<std::chrono::seconds>(zt.get_sys_time());
            std::stringstream ss;

            if (filename)
                ss << std::format("{:%Y-%m-%d_%H-%M-%S}", floored_time);
            else
                ss << std::format("{:%Y-%m-%d %H:%M:%S}", floored_time);

            return ss.str();
        }

        // Returns a string with the date in the current timezone - (YYYY-MM-DD)
        static std::string Date()
        {
            auto now = std::chrono::system_clock::now();
            auto time_zone = std::chrono::current_zone();
            auto zt = std::chrono::zoned_time{ time_zone, now };
            std::stringstream ss;

            ss << std::format("{:%Y-%m-%d}", zt);

            return ss.str();
        }

        // Returns a string with the time in the current timezone - (HH:MM:SS)
        static std::string Time()
        {
            auto now = std::chrono::system_clock::now();
            auto time_zone = std::chrono::current_zone();
            auto zt = std::chrono::zoned_time{ time_zone, now };
            auto floored_time = std::chrono::floor<std::chrono::seconds>(zt.get_sys_time());
            std::stringstream ss;

            ss << std::format("{:%H:%M:%S}", floored_time);

            return ss.str();
        }
    }
}
