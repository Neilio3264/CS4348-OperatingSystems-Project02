#include "../../include/Log/log_stream.h"
#include "../../include/Log/logger.h"

namespace zotikos
{
    logStream::logStream(logger &log, log_state state) : _logger(log), _state(state)
    {
    }

    logStream::~logStream()
    {
        _logger.push(get_time() + get_state() + str());
    }

    const std::string logStream::get_state()
    {
        std::string action;
        switch (_state)
        {
        case log_state::BEGIN:
            action = "[START] ";
            break;

        case log_state::PASS_SET:
            action = "[PASSKEY] ";
            break;

        case log_state::ENCRYPT:
            action = "[ENCRYPT] ";
            break;

        case log_state::DECRYPT:
            action = "[DECRYPT] ";
            break;

        case log_state::QUIT:
            action = "[STOP] ";
            break;

        case log_state::RESULT:
            action = "[RESULT] ";
            break;

        case log_state::ERROR:
            action = "[ERROR] ";
            break;
        }

        return action;
    }

    const std::string logStream::get_time()
    {
        return zotikos::getDateTimeStr();
    }
}