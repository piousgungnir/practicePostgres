#pragma once


#include <chrono>
#include <string>

using Clock = std::chrono::system_clock;
using Timestamp = Clock::time_point;


inline std::string timestamp_to_str(const std::string & format, const Timestamp & timestamp)
{
    std::time_t t = std::chrono::system_clock::to_time_t(timestamp);

    char mbstr[24];
    if (std::strftime(mbstr, sizeof(mbstr), format.c_str(), std::gmtime(&t))) {
        return mbstr;
    }
    return {};
}


inline std::string timestamp_to_str_datetime(const Timestamp & timestamp)
{
    return timestamp_to_str("%H:%M:%S %d.%m.%Y", timestamp);
}


inline std::string timestamp_to_str_timeonly(const Timestamp & timestamp)
{
    return timestamp_to_str("%H:%M:%S", timestamp);
}

inline std::string timestamp_to_str_timeonly_with_ms(const Timestamp & timestamp)
{
    auto timeStr = timestamp_to_str("%H:%M:%S", timestamp);
    auto tpSec = std::chrono::time_point_cast<std::chrono::seconds>(timestamp);
    auto tpMs = std::chrono::time_point_cast<std::chrono::milliseconds>(timestamp);
    auto ms= (tpMs - tpSec).count();
    auto msStr = std::to_string(ms);
    msStr = std::string(3 - msStr.size(), '0') + msStr; // строка должна иметь 3 символа, чтобы корректно интерпретировать значение милисекунд
    timeStr += "." + msStr;
    return timeStr;
}

inline std::string timestamp_to_str_dateonly(const Timestamp & timestamp)
{
    return timestamp_to_str("%d.%m.%Y", timestamp);
}
