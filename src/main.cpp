/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:05:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/18 16:23:45 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "configParser.hpp"
#include "defines.hpp"
#include "webservException.hpp"
#include <iostream>


// todo
// make headers understandable, where we actually need them

void	parseConfigFile(std::string const& filename) {

	ConfigParser		configParser;

	configParser.lex(filename);
	configParser.parse();
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		std::cerr << RED << "Invalid number of arguments\nUsage: ./webserv [path to configuration file, ending with .conf]\n";
		return 1;
	}


	try {
		parseConfigFile(std::string(argv[1]));
		Webserv	webserver;
		webserver.run();
	}
	catch (WebservException& e) {
		std::cerr << e.what();
		return 1;
	}

//	Server server;
//
//	server.run();
//	return (0);
}
