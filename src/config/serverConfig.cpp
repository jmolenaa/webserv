/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serverConfig.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 11:06:40 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/22 11:42:50 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "serverConfig.hpp"
#include "defines.hpp"
#include "arpa/inet.h"

ServerConfig::ServerConfig() : _port(htons(PORT)), _address(htonl(INADDR_ANY)), _name("default")
{
	_locations["root"] = Location();
}

uint16_t ServerConfig::getPort() const
{
	return this->_port;
}

std::string ServerConfig::getName() const
{
	return this->_name;
}

uint32_t ServerConfig::getAddress() const
{
	return this->_address;
}

Location ServerConfig::matchLocation(std::string path) const
{
	auto it = _locations.find(path);
	if (it != _locations.end())
		return (it->second);
	return _locations.begin()->second;
}
