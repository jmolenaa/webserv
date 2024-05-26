/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:05:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/24 14:02:28 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "configParser.hpp"
#include "defines.hpp"
#include "webservException.hpp"
#include <iostream>
#include "log.hpp"

// todo
// make headers understandable, where we actually need them
// change localhost to default

void	parseConfigFile(std::string const& filename) {
	ConfigParser	configParser("no");
	ConfigParser	test(configParser);

	configParser.lex(filename);
	configParser.parse();
}

int main(int argc, char *argv[])
{
	(void)argv;
	if (argc != 2) {
		std::cerr << RED << "Invalid number of arguments\nUsage: ./webserv [path to configuration file, ending with .conf]\n";
		return EXIT_FAILURE;
	}


	try {
		parseConfigFile(std::string(argv[1]));
		exit(0);
		Log::getInstance().enableLog(true); //comment out to disable logging.
		
		// parseConfigFile(std::string(argv[1]));
		Webserv	webserver;
		webserver.run();
	}
	catch (WebservException& e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}

//	Server server;
//
//	server.run();
	return EXIT_SUCCESS;
}
