#ifndef ZOTIKOS_LOG_STREAM_H
#define ZOTIKOS_LOG_STREAM_H

#include "./timeutil.h"
#include "./log_states.h"

namespace zotikos
{
    class logger;

    class logStream : public std::ostringstream
    {
        logger &_logger;
        log_state _state;

        const std::string get_state();
        const std::string get_time();

    public:
        logStream(logger &log, log_state);
        logStream(const logStream &obj);
        ~logStream();
    };
}

#endif