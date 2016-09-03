/*
 * config.hpp
 *
 *  Created on: Jun 10, 2016
 *      Author: hmeyer
 */

#ifndef CONFIG_HXX_
#define CONFIG_HXX_


struct Config {
	int port_num = 8888;
	char* data = nullptr;
};

enum class ImmediateAction {
	none,
	help,
	version
};

#endif /* CONFIG_HXX_ */
