/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:05:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/28 12:45:44 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "defines.hpp"
#include "webservException.hpp"
#include <iostream>
#include "log.hpp"
#include "restaurant.hpp"

// todo
// make headers understandable, where we actually need them
// change localhost to default

int main(int argc, char *argv[])
{
	if (argc > 2) {
		std::cerr << RED << "Invalid number of arguments\nUsage: ./restaurant [page to configuration file, ending with .conf]\nOr ./restaurant to start with default configuration\n";
		return EXIT_FAILURE;
	}

	std::string	filename("configFiles/default.conf");
	if (argc == 2) {
		filename = argv[1];
	}
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
