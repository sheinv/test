// test_prj.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include <csignal>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <boost/log/expressions.hpp>

#include <boost/log/utility/setup/console.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sinks/async_frontend.hpp>
//#include <boost/log/utility/setup/file.hpp>

#include "proxy_srv.h"
#include "logger.h"

namespace logging = boost::log;
namespace sinks   = logging::sinks;

BOOL CtrlHandler( DWORD fdwCtrlType )
{
  switch( fdwCtrlType )
  {
    case CTRL_C_EVENT:
      printf( "Ctrl-C event\n\n" );
      //Beep( 750, 300 );
      std::raise(SIGINT);
      return( TRUE );

    case CTRL_CLOSE_EVENT:
      //Beep( 600, 200 );
      printf( "Ctrl-Close event\n\n" );
      std::raise(SIGTERM);
      return( TRUE );

    case CTRL_BREAK_EVENT:
      //Beep( 900, 200 );
      printf( "Ctrl-Break event\n\n" );
      std::raise(SIGINT);
      return TRUE;

    case CTRL_LOGOFF_EVENT:
      //Beep( 1000, 200 );
      printf( "Ctrl-Logoff event\n\n" );
      std::raise(SIGINT);
      return TRUE;

    case CTRL_SHUTDOWN_EVENT:
      //Beep( 750, 500 );
      printf( "Ctrl-Shutdown event\n\n" );
      std::raise(SIGTERM);
      return TRUE;

    default:
      return FALSE;
  }
}

void handler(
        const boost::system::error_code& error,
        int signal_number,
        boost::asio::io_context& _io_ctx)
{
    if (error)
        throw std::runtime_error(error.message());
    {
        switch (signal_number)
        {
            case SIGTERM:
                _io_ctx.stop();
                break;
                //            case SIGBREAK:
                //                _io_ctx.stop();
                //                break;
            case SIGINT:
                _io_ctx.stop();
                break;
            default:
                break;
        }
    }
}

int main(int ac, char **av)
{
    using namespace proxy;
    using namespace logger;

    namespace bpo = boost::program_options;

    if (!SetConsoleCtrlHandler( (PHANDLER_ROUTINE) CtrlHandler, TRUE ))
    {
        std::cerr << "Couldn't set console control handler. Exit." << std::endl;
        return -1;
    }

    try {
        bas::io_context io_ctx;

        bas::signal_set signals(io_ctx
                                , SIGTERM
                                //, SIGBREAK
                                , SIGINT
                                );

        signals.async_wait(std::bind(handler, std::placeholders::_1, std::placeholders::_2, std::ref(io_ctx)));

        bool is_verbose {false};

        // Declare the supported options.
        bpo::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "produce help message")
                ("dport", bpo::value<int>()->default_value(1321), "downlink port")
                ("daddr", bpo::value<std::string>()->default_value("0.0.0.0"), "set downlink address")
                ("uport", bpo::value<int>()->default_value(1433), "uplink port")
                ("uaddr", bpo::value<std::string>()->default_value("127.0.0.1"), "set uplink port")
                ("verbose,v", bpo::bool_switch(&is_verbose), "set uplink port")
                ("logfile", bpo::value<std::string>()->default_value("test.log"), "logfile name")
                ;

        bpo::variables_map vm;
        bpo::store(bpo::parse_command_line(ac, av, desc), vm);
        bpo::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
        }
        else
        {
            min_severity_filter min_severity = expr::channel_severity_filter(channel, severity);

            // Set up the minimum severity levels for different channels
            min_severity["console"] = error;
            min_severity["file"] = trace;

            if (is_verbose)
            {
                min_severity["console"] = trace;
                min_severity["file"] = trace;
            }

            init(min_severity, vm["logfile"].as<std::string>());

//            BOOST_LOG_SEV(lg_console, error) << "A normal severity level message";
//            BOOST_LOG_SEV(lg_file, trace) << "A warning severity level message";

            std::cout << "Press Ctrl+C to stop the application" << std::endl;

            proxy_srv srv(
                    io_ctx,
                    vm["daddr"].as<std::string>(),
                    vm["dport"].as<int>(),
                    vm["uaddr"].as<std::string>(),
                    vm["uport"].as<int>()
                    );

            io_ctx.run();
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

