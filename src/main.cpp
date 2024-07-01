/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:05:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/14 17:15:42 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "defines.hpp"
#include "webservException.hpp"
#include <iostream>
#include "log.hpp"
#include "restaurant.hpp"
#include <signal.h>

volatile sig_atomic_t interrupted = 0;

void	sigIntHandler(int signal) {
	(void)signal;
	interrupted = 1;
}

int main(int argc, char *argv[])
{
	if (argc > 2) {
		std::cerr << RED << "Invalid number of arguments\nUsage: ./webserv [page to configuration file, ending with .conf]\nOr ./webserv to start with default configuration\n";
		return EXIT_FAILURE;
	}

	std::string	filename("configFiles/default.conf");
	if (argc == 2) {
		filename = argv[1];
	}
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, sigIntHandler);
	try {
		Log::getInstance().enableLog(true); //comment out to disable logging.
		
		Restaurant	restaurant(filename);
		restaurant.run();
	}
	catch (WebservException& e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
