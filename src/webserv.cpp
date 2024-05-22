/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webserv.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/25 17:09:43 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 11:34:09 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

//program stuff goes here, such as startup, setup and terminating etc

//Webserv::Webserv() {
//
//}

void Webserv::run() {

	Epoll epoll;
	ServerConfig config;

	Server	serve(epoll, config);
//	std::vector<Server>	servers({serve});
//	servers.push_back(*serve);
//	std::cout << &serve << "\n";
//	std::cout << &servers[0] << "\n";

	serve.run();
//	for (size_t i = 0; i < servers.size(); ++i) {
//		this->_servers[i].run();
//	}
//	std::cerr << "hey\n";
}
