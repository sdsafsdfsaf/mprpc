#include "logger.h"
Logger &Logger::GetInstance()
{
    static Logger app;
    return app;
}

void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}

void Logger::Log(const std::string &msg)
{
    m_lckQue.Push(msg);
}

Logger::Logger()
{
    std::thread writeLogTask([&]()
                             {
        for (;;)
        {
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);
            char file_name[128] = {0};
            sprintf(file_name, "%d-%d-%d-log.txt", nowtm->tm_year + 1900, nowtm->tm_mon + 1, nowtm->tm_mday);
            FILE *pf = fopen(file_name, "a+"); //a+的意思 //TODO
            if (pf == nullptr)
            {
                std::cout << "logger file : " << file_name << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::string msg = m_lckQue.Pop();
            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d =>[%s]", nowtm->tm_hour, nowtm->tm_min, nowtm->tm_sec,
                                                 (m_loglevel == INFO ? "info" : "error")); //FIXME 日志级别运行时应该会存在问题，可以在放入队列时就加入日志级别信息
            msg.insert(0, time_buf);
            msg.append("\n");
            fputs(msg.c_str(), pf);
            fclose(pf);
        } });
    writeLogTask.detach();
}
