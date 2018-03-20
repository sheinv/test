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

void init(const min_severity_filter &min_severity, const std::string& file_name)
{
    auto LogFormat {expr::format("%1%: <%2%> %3%") % line_id % severity % expr::smessage};

    // Create a minimal severity table filter
    logging::add_console_log
            (
                std::clog,
                keywords::filter = (min_severity || severity >= critical) && (channel == "console"),
                keywords::format = LogFormat
           );

    auto core = logging::core::get();

    // Create a text file sink
    using tf_backend_t = sinks::text_file_backend;
    using tf_sink_t = sinks::asynchronous_sink<tf_backend_t>;

    auto tf_backend = boost::make_shared<tf_backend_t>(
        keywords::file_name = file_name,
        keywords::rotation_size = 5 * 1024 * 1024,
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(12, 0, 0)
    );

    auto tf_sink = boost::make_shared<tf_sink_t>(tf_backend);

    tf_sink->set_filter((min_severity || severity >= critical) && (channel == "file"));

    tf_sink->set_formatter(LogFormat);

    // Add it to the core
    core->add_sink(tf_sink);

    logging::add_common_attributes();
}

}
