#pragma once

#ifndef LOGGER_H
#define LOGGER_H

#include <cstddef>
#include <iostream>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logger {

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

// We define our own severity levels
enum severity_level
{
    trace,
    warning,
    error,
    critical
};

using min_severity_filter = expr::channel_severity_filter_actor< std::string, severity_level > ;

// Define the attribute keywords
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)

std::ostream& operator<< (std::ostream& strm, severity_level level);

void init(const min_severity_filter& min_severity);

// Define our logger type
using logger_t = src::severity_channel_logger< severity_level, std::string >;

}
#endif // LOGGER_H
