// log.cpp
#include "log.h"
#include "mem.h"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include <memory>
#include <sstream>
#include <cstdio>

namespace core
{

std::string getTimeLogString()
{
    const size_t buffSize = 32;
    char buffTime[buffSize] = { 0 };

    struct timeval tv;
    gettimeofday(&tv, 0);
    struct tm * ptm = localtime(&tv.tv_sec);

    snprintf(buffTime, buffSize,"%04d.%02d.%02d %02d:%02d:%02d.%03d  ",
                ptm->tm_year + 1900,
                ptm->tm_mon + 1,
                ptm->tm_mday,
                ptm->tm_hour,
                ptm->tm_min,
                ptm->tm_sec, (int)tv.tv_usec/1000);

    return std::string(buffTime);
}

std::string dumpPacket(const char* apBuff, size_t len)
{
    const size_t countBytesPerLine = 16; // 16 byte on one line
    const size_t halfCountBytes = countBytesPerLine/2;

    const size_t buffSize = countBytesPerLine * 3 + 5 + 2 + 1;
    const size_t buffSymbsSize = countBytesPerLine + 3;

    char buff[buffSize] = { 0 };
    char buffSymbs[buffSymbsSize] = { 0 }; // prepare buff "| count byte     |"

    std::stringstream ss;

    buffSymbs[0] = '|';
    buffSymbs[countBytesPerLine + 1] = '|';

    size_t blocks = len/countBytesPerLine;
    if ((len % countBytesPerLine) != 0)
    {
        blocks += 1;
    }

    ss << string_format("Block(%d):\n",len);
    for (size_t i = 0; i < blocks; i++ )
    {
        size_t ilen = 0;
        ilen += std::snprintf(buff + ilen,buffSize - ilen, "%04X:", static_cast<unsigned int>(i * countBytesPerLine));
        for (size_t j = 0; j < countBytesPerLine; j++)
        {
             size_t idx = i * countBytesPerLine + j;
             if (j != 0 && (j % halfCountBytes) == 0)
             {
                 char sep = ' ';
                 if ((idx + 1) < len) // check there is next element
                 {
                     sep = '-';
                 }
                 ilen += std::snprintf(buff + ilen,buffSize - ilen, " %c", sep);
             }
             if (idx < len)
             {
                 unsigned char c = static_cast<unsigned char>(apBuff[idx]);
                 ilen += std::snprintf(buff + ilen,buffSize - ilen, " %02X", c);
                 buffSymbs[j + 1] = ((iscntrl(c) || c >= 0x80) ? '.' : c);
             }
             else
             {
                 ilen += std::snprintf(buff + ilen,buffSize - ilen, "   ");
                 buffSymbs[j + 1] = ' ';
             }
        }
        ss << std::string(buff) << "  " << std::string(buffSymbs) << std::endl;
    }
    return ss.rdbuf()->str();
}

void Log(int logLevel, const std::string& message)
{
    std::string s = getTimeLogString();
    s += message;
    s += std::string("\n");

    write(STDOUT_FILENO, s.c_str(), s.length());
    fsync(STDOUT_FILENO);
}


} // namespace
