#include "../../include/Log/logger.h"

#include <iostream>

namespace zotikos
{
    logger::logger()
    {
        _print_thread = std::thread(print, this, std::chrono::milliseconds(16));
    }

    logger::~logger()
    {
        _print = false;
        if (_print_thread.joinable())
        {
            _print_thread.join();
        }
    }

    void logger::push(std::string msg)
    {
        std::lock_guard<std::mutex> lock(_queue_mutex);
        _queue.push(std::move(msg));
    }

    void logger::print(logger *instance, std::chrono::duration<double, std::milli> interval)
    {
        while (instance->_print || !instance->_queue.empty())
        {
            auto t1 = std::chrono::steady_clock::now();
            {
                std::lock_guard<std::mutex> lock(instance->_queue_mutex);
                while (!instance->_queue.empty())
                {
                    std::cout << instance->_queue.front() << std::endl;
                    instance->_queue.pop();
                }
            }
            auto t2 = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> time_took = t2 - t1;
            // sleep
            if (time_took < interval && instance->_print)
            {
                std::this_thread::sleep_for(interval - time_took);
            }
        }
    }
}