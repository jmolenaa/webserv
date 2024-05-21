/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   serverSettings.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 11:06:40 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/08 11:06:40 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//
// Created by jmolenaa on 8-5-24.
//

#include "serverSettings.hpp"
#include "defines.hpp"
#include "arpa/inet.h"

ServerSettings::ServerSettings() : _port(htons(PORT)), _address(htonl(INADDR_ANY)), _name("localhost") {
	ServerSettings::loc	defaultLoc;

	defaultLoc._path = "";
	defaultLoc._root = "root";
	defaultLoc._index = "index.html";
	defaultLoc._allowedMethods = GET | POST | DELETE;
	defaultLoc._maxBodySize = MAX_BODY_SIZE;
	defaultLoc._autoindex = false;
	this->_locs.push_back(defaultLoc);
}

std::vector<ServerSettings::loc> ServerSettings::getLocs() const {
	return this->_locs;
}

uint16_t ServerSettings::getPort() const {
	return this->_port;
}

std::string ServerSettings::getName() const {
	return this->_name;
}

uint32_t ServerSettings::getAddress() const {
	return this->_address;
}

