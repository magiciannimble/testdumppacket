// log.h
#ifndef __LOG_H__
#define __LOG_H__

#include <string>

namespace core
{

std::string dumpPacket(const char* apBuff, size_t len);

std::string getTimeLogString();

template<typename ... Args>
std::string string_format(const std::string& format, const Args& ... args)
{
    size_t size = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // 1 Extra space for '\0'
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

template<typename... Args>
void Log(int logLevel, const std::string& format, const Args& ... args)
{
    std::string s = getTimeLogString();
    s += string_format(format, args ...);
    s += std::string("\n");

    write(STDOUT_FILENO, s.c_str(), s.length());
    fsync(STDOUT_FILENO);
}

void Log(int logLevel, const std::string& message);

}
#endif // __LOG_H__
