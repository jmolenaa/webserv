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

// TODO
// Gonna have to make the unordered maps we had into normal maps, so the default recipe and cook are actually at the beginning lol
// Possibly rearrange where the port and host are held, since the waiter now has a Kitchen of same port and hosts
// but we need to go into the kitchen to the first cook to actually find the host or port we need
void	Restaurant::oldInit() {
	// parseConfig(std::string(configFileName));
	//Move this into parsing. Could be a local class / private struct. This could all be garbage. Change as needed
		//in some nested while loop based on Table number / address
			Kitchen kitchen;
			std::string cookName = "localhost"; //AKA "server_name"
			Cookbook cookbook;

			//int another loop
			//{
				Recipe	recipe; //self-initialises with defaults
				std::string	page = "root"; //extract from config
//				update recipe stuff here where needed
				//e.g. loc.page = locationPath
				//e.g. loc.index = lineFromFile.substr(start, size) whatever etc;
				cookbook[page] = recipe;

				//replace PORT and INADDR_ANY maybe use getaddrinfo and freeaddrinfo for the default values?
				kitchen[cookName] = Cook(htons(PORT), htonl(INADDR_ANY), cookName, cookbook);
			//}
				Waiter*	waiter = new Waiter(_epoll, kitchen);
				_waiters.push_back(waiter);
	//}

}

//program stuff goes here, such as startup, setup and terminating etc
Restaurant::Restaurant(std::string const& filename)
{
	Log::getInstance().print("Constructing Restaurant instance");
	Log::getInstance().print("Parsing '" + filename + "'...jk");

	// if something broke after this just comment out the initialiseWaiters and uncomment oldInit
	this->_initialiseWaiters(filename);
//	oldInit();
}

void Restaurant::_initialiseWaiters(const std::string &filename) {
	Menu	menu(filename);

	for (Kitchen const& kitchen : menu.getKitchens()) {
		this->_waiters.push_back(new Waiter(this->_epoll, kitchen));
	}
}

void Restaurant::run()
{
	for (auto& waiter : _waiters)
		waiter->work();
}