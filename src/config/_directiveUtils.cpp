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
		std::cout << "found Ip address and port in directive\n";
	}
	else if (ipAndPort.find('.') != std::string::npos) {
		newIp = ipAndPort;
		std::cout << "found Ip address in directive\n";
	}
	else {
		newPort = ipAndPort;
		std::cout << "found port in directive\n";
	}
}

void Menu::setNewIp(const std::string &newIp) {
	(void)newIp;
}

void Menu::setNewPort(const std::string &newPort) {
	(void)newPort;
}
