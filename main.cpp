// test_prj.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>
#include <boost/log/expressions.hpp>

#include "proxy_srv.h"

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
				//			case SIGBREAK:
				//				_io_ctx.stop();
				//				break;
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

	namespace bpo = boost::program_options;

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
				("help", "produce help message")
				("dport", bpo::value<int>()->default_value(1321), "downlink port")
				("daddr", bpo::value<std::string>()->default_value("0.0.0.0"), "set downlink address")
				("uport", bpo::value<int>()->default_value(1433), "uplink port")
				("uaddr", bpo::value<std::string>()->default_value("127.0.0.1"), "set uplink port")
				("verbose,v", bpo::bool_switch(&is_verbose), "set uplink port")
				;

		bpo::variables_map vm;
		bpo::store(bpo::parse_command_line(ac, av, desc), vm);
		bpo::notify(vm);

		if (vm.count("help")) {
			std::cout << desc << std::endl;
		}
		else
		{
			if (!is_verbose)
					boost::log::core::get()->set_filter
					(
						boost::log::trivial::severity >= boost::log::trivial::error
					);

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
		BOOST_LOG_TRIVIAL(info) << __FUNCSIG__ << e.what();
	}

	std::cin.get();

	return 0;
}

