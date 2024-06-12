/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   restaurant.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 17:09:43 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 15:25:25 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "restaurant.hpp"
#include "log.hpp"
#include "menu.hpp"

// void	Restaurant::oldInit() {
// 	// parseConfig(std::string(configFileName));
// 	//Move this into parsing. Could be a local class / private struct. This could all be garbage. Change as needed
// 		//in some nested while loop based on Table number / address
// 			Kitchen kitchen;
// 			std::string cookName = "localhost"; //AKA "server_name"
// 			Cookbook cookbook;

// 			//int another loop
// 			//{
// 				Recipe	recipe; //self-initialises with defaults
// 				std::string	page = "root"; //extract from config
// //				update recipe stuff here where needed
// 				//e.g. loc.page = locationPath
// 				//e.g. loc.index = lineFromFile.substr(start, size) whatever etc;
// 				cookbook[page] = recipe;

// 				//replace PORT and INADDR_ANY maybe use getaddrinfo and freeaddrinfo for the default values?
// 				kitchen[cookName] = Cook(htons(PORT), htonl(INADDR_ANY), cookName, cookbook);
// 			//}
// 				Waiter*	waiter = new Waiter(_epoll, kitchen);
// 				_waiters.push_back(waiter);
// 	//}

// }

//program stuff goes here, such as startup, setup and terminating etc
Restaurant::Restaurant(std::string const& filename)
{
	Log::getInstance().print("Constructing Restaurant instance");
	Log::getInstance().print("Parsing '" + filename + "'...jk");

	// if something broke after this just comment out this stuff and uncomment oldInit
	Menu	menu(filename);

	for (Kitchen const& kitchen : menu.getKitchens()) {
		this->_waiters.push_back(new Waiter(this->_epoll, kitchen));
	}
	// oldInit();
}

void Restaurant::run()
{
	//start epoll loop here
	for (auto& waiter : _waiters)
		waiter->work();
}