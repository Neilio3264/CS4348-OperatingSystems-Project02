#ifndef ZOTIKOS_TIMEUTIL_H
#define ZOTIKOS_TIMEUTIL_H

#include <sstream>
#include <ctime>

namespace zotikos
{
    // YYYY-MM-DD HH:MM
    inline std::string formatDateTime(const tm &tm)
    {
        std::stringstream ss;

        // date
        ss << tm.tm_year + 1900 << "-";
        if (tm.tm_mon + 1 < 10)
            ss << "0" << tm.tm_mon + 1 << "-";
        else
            ss << tm.tm_mon + 1 << "-";
        if (tm.tm_mday < 10)
            ss << "0" << tm.tm_mday << " ";
        else
            ss << tm.tm_mday << " ";

        // time
        ss << tm.tm_hour << ":";
        if (tm.tm_min < 10)
            ss << "0" << tm.tm_min << " ";
        else
            ss << tm.tm_min << " ";

        return ss.str();
    }

    // returns the current date and time: // YYYY-MM-DD HH:MM
    inline std::string getDateTimeStr()
    {
        time_t t = time(NULL);

        tm now = *std::localtime(&t);
        return formatDateTime(now);
    }
}

#endif