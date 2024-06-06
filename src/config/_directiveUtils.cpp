/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _directiveUtils.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/03 14:54:16 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/06/03 14:54:16 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "menu.hpp"
#include <algorithm>
#include <cmath>
#include <limits>

std::string Menu::popFrontToken() {
	std::string	nextToken = this->getTokens().front();
	this->getTokens().pop_front();
	return nextToken;
}

int Menu::getArrayIndex(std::string const& errorCode) {

	for (size_t i = 0; i < COUNT; ++i) {
		if (errorCode == this->_errorCodesArray[i]) {
			return i;
		}
	}
	return 0;
}

short Menu::identifyMethod(const std::string &method) {
	if (method == "GET") {
		return GET;
	}
	else if (method == "POST") {
		return POST;
	}
	else if (method == "DELETE") {
		return DELETE;
	}
	return 0;
}

void Menu::splitAndInitialiseIpAndPortStrings(const std::string &ipAndPort, std::string &newIp, std::string& newPort) {
	size_t	posInString = ipAndPort.find(':');
	if (posInString != std::string::npos) {
		newIp = ipAndPort.substr(0, posInString);
		newPort = ipAndPort.substr(posInString + 1, std::string::npos);
	}
	else if (ipAndPort.find('.') != std::string::npos) {
		newIp = ipAndPort;
	}
	else {
		newPort = ipAndPort;
	}
}

static int	convertStringToNumber(std::string const& string, std::string const& errorStr, int limit) {
	int	number = 0;

	if (string.find_first_not_of("0123456789") != std::string::npos) {
		throw WebservException("Webserv: configuration file: encountered invalid character in '" + errorStr + "' in 'listen' directive\n");
	}
	if (static_cast<float>(string.length()) > std::log10(limit) + 1) {
		throw WebservException("Webserv: configuration file: one of the numbers in '" + errorStr + "' is out of bounds, should be between 0 and " + std::to_string(limit) + "\n");
	}
	number = std::stoi(string);
	if (number > limit) {
		throw WebservException("Webserv: configuration file: one of the numbers in '" + errorStr + "' is out of bounds, should be between 0 and " + std::to_string(limit) + "\n");
	}
	return number;
}

void Menu::setNewIp(const std::string &newIp, std::string const& ipAndPort) {
	// the if statement is when the listen directive does not contain an IP address, so we set it to default
	if (newIp.empty()) {
		this->getCurrentCook()->setAddress(htonl(INADDR_ANY));
		return ;
	}
	if (std::count(newIp.begin(), newIp.end(), '.') != 3) {
		throw WebservException("Webserv: configuration file: incorrect amount of octets in '" + ipAndPort + "' in 'listen' directive\n");
	}

	uint32_t	newIpAddress = 0;
	int			bitShift = 24;
	size_t	i = 0;
	// loops over the dots in the string and converts the octets to numbers then bitshifts them into the new address
	for (size_t start = 0; i != std::string::npos && bitShift >= 0; start = i + 1) {
		i = newIp.find('.', start);
		int currentOctet = convertStringToNumber(newIp.substr(start, i - start), ipAndPort, 255);
		newIpAddress |= currentOctet << bitShift;
		bitShift -= 8;
	}
	this->getCurrentCook()->setAddress(htonl(newIpAddress));
}

void Menu::setNewPort(const std::string &newPort, std::string const& ipAndPort) {
	int	newPortNumber;

	// the if statement is when the listen directive does not contain a port, so we set it to default
	if (newPort.empty()) {
		newPortNumber = PORT;
	}
	else {
		newPortNumber = convertStringToNumber(newPort, ipAndPort, std::numeric_limits<uint16_t>::max());
	}
	this->getCurrentCook()->setTable(htons(newPortNumber));
}
