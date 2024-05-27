/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:05:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 16:41:24 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "defines.hpp"
#include "webservException.hpp"
#include "log.hpp"
#include "restaurant.hpp"

int main(int argc, char *argv[])
{
	(void)argv;
	if (argc != 2) {
		std::cerr << RED << "Invalid number of arguments\nUsage: ./restaurant [path to configuration file, ending with .conf]\n";
		return EXIT_FAILURE;
	}

	try {
		Log::getInstance().enableLog(true); //comment out to disable logging.
		
		Restaurant	restaurant(argv[1]);
		restaurant.run();
	}
	catch (WebservException& e) {
		std::cerr << e.what();
		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}
