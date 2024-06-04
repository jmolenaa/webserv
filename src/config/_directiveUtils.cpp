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