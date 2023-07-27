#pragma once
#include "lockqueue.h"
#include <string>
#include <time.h>
#include <iostream>
#include <thread>

#define LOG_INFO(logmsgformat, ...)                     \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::GetInstance();         \
        logger.SetLogLevel(INFO);                       \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
    } while (0)

#define LOG_ERR(logmsgformat, ...)                      \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::GetInstance();         \
        logger.SetLogLevel(ERROR);                      \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
    } while (0)

enum LogLevel
{
    INFO,
    ERROR,
};

class Logger
{
public:
    static Logger &GetInstance();
    void SetLogLevel(LogLevel level);
    void Log(const std::string &); // ¼Ó²»¼Óconst£¿ //FIXME

private:
    Logger();
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    int m_loglevel;
    LockQueue<std::string> m_lckQue;
};
