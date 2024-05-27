/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 17:09:43 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 13:09:52 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "log.hpp"

//program stuff goes here, such as startup, setup and terminating etc
Webserv::Webserv(std::string configFileName)
{
	Log::getInstance().print("Constructing Webserv instance");
	Log::getInstance().print("Parsing '" + configFileName + "'...jk");

	// parseConfig(std::string(configFileName));
	//Move this into parsing. Could be a local class / private struct. This could all be garbage. Change as needed
		//in some nested while loop based on Port number / address
			ServerConfigs serverConfigs;
			std::string serverConfigName = "localhost"; //AKA "server_name"
			Locations serverConfigLocations;

			//int another loop
			//{
				Location	loc; //self-initialises with defaults
				std::string	locationPath = "root"; //extract from config
				//update location stuff here where needed
				//e.g. loc.path = locationPath
				//e.g. loc.index = lineFromFile.substr(start, size) whatever etc;
				serverConfigLocations[locationPath] = loc;
		
				//replace PORT and INADDR_ANY
				serverConfigs[serverConfigName] = ServerConfig(htons(PORT), htonl(INADDR_ANY), serverConfigName, serverConfigLocations);
			//}
				Server*	s = new Server(_epoll, serverConfigs);
				_servers.push_back(s);
	//}
}

void Webserv::run()
{
	for (auto& server : _servers)
		server->run();
}