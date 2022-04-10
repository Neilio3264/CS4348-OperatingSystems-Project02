#ifndef ZOTIKOS_LOG_STATES_H
#define ZOTIKOS_LOG_STATES_H

namespace zotikos
{
    enum class log_state
    {
        BEGIN,
        PASS_SET,
        ENCRYPT,
        DECRYPT,
        QUIT,
        RESULT,
        ERROR
    };
}

#endif