#ifndef ZOTIKOS_LOGGER_H
#define ZOTIKOS_LOGGER_H

#include "./log_states.h"
#include "./log_stream.h"

#include <queue>
#include <mutex>
#include <thread>
#include <chrono>
#include <fstream>

namespace zotikos
{
    class logger
    {
        std::queue<std::string> _queue;
        std::mutex _queue_mutex;

        std::mutex _stdout_mutex;
        std::mutex _stderr_mutex;

        std::thread _print_thread;
        bool _print = true;
        static void print(logger *instance, std::chrono::duration<double, std::milli> interval);

        std::string _fileName;

        void setFile(std::string fileName)
        {
            _fileName = fileName;
        }

        logger();
        ~logger();

    public:
        logger(logger const &) = delete;
        void operator=(logger const &) = delete;

        static logStream log(log_state state = log_state::BEGIN)
        {
            static logger _instance;
            _instance.setFile("../Log/run.log");
            return logStream(_instance, state);
        }

        void push(std::string msg);
    };
}

#endif