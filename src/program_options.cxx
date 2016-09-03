/*
 * program_options.cxx
 *
 *  Created on: Jun 10, 2016
 *      Author: hmeyer
 */

#ifdef _GLIBCXX_DEBUG
#undef _GLIBCXX_DEBUG
#endif

#include "config.hxx"

#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>

namespace opt {

namespace {
const char* help = "help";
const char* port = "port";
}
}

ImmediateAction adjust_config(Config& config, int argc, char*const* argv) {
	using namespace boost::program_options;

	// Declare the supported options.
	options_description desc("Allowed options");
	desc.add_options() \
			(opt::help, "produce help message") \
			(opt::port, value(&config.port_num)->default_value(config.port_num), "server port") \
			;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	if (vm.count(opt::help)) {
		std::ostringstream os;
		os << desc;
		std::string s = os.str();
		config.data = new char[1 + s.size()];
		memcpy(config.data, s.c_str(), s.size());
		config.data[s.size()] = 0;
		return ImmediateAction::help;
	}

	return ImmediateAction::none;
}
