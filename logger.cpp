#include "logger.h"

namespace logger {

std::ostream &operator<<(std::ostream &strm, severity_level level)
{
    static const char* strings[] =
    {
        "trace",
        "warning",
        "error",
        "critical"
    };

    if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
        strm << strings[level];
    else
        strm << static_cast< int >(level);

    return strm;
}

void init(const min_severity_filter &min_severity)
{
    // Create a minimal severity table filter
    logging::add_console_log
            (
                std::clog,
                keywords::filter = (min_severity || severity >= critical) && (channel == "console"),
                keywords::format =
                (
                    expr::stream
                    << line_id
                    << ": <" << severity
                    << "> [" << channel << "] "
                    << expr::smessage
                )
           );



    logging::add_common_attributes();
}

}
