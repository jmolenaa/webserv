/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serverConfig.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 11:06:40 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/27 13:13:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>

#include "serverConfig.hpp"
#include "defines.hpp"
#include "log.hpp"

ServerConfig::ServerConfig()
	: _port(htons(PORT)), _address(htonl(INADDR_ANY)), _name("localhost")
{
	Log::getInstance().print("default ServerConfig constructed with name: " + _name);
}

ServerConfig::ServerConfig(uint16_t port, uint32_t address, std::string& name, Locations& locations)
	: _port(port), _address(address), _name(name), _locations(locations)
{
	Log::getInstance().print("custom ServerConfig constructed with name: " + _name);
}

ServerConfig::ServerConfig(const ServerConfig& other)
{
	if (this == &other)
		return;
	this->_port = other.getPort();
	this->_address = other.getAddress();
	this->_name = other.getName();
	this->_locations = other.getLocations();
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other)
{
	if (this == &other)
		return (*this);
	this->_port = other.getPort();
	this->_address = other.getAddress();
	this->_name = other.getName();
	this->_locations = other.getLocations();
	return (*this);
}

uint16_t ServerConfig::getPort() const
{
	return this->_port;
}

uint32_t ServerConfig::getAddress() const
{
	return this->_address;
}

std::string ServerConfig::getName() const
{
	return this->_name;
}

Locations ServerConfig::getLocations() const
{
	return this->_locations;
}

Location ServerConfig::matchLocation(std::string path) const
{
	auto it = _locations.find(path);
	if (it != _locations.end())
		return (it->second);
	return _locations.begin()->second;
}
