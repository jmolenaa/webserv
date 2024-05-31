/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _errorCheckingFunctions.cpp                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/20 14:41:00 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/20 14:41:00 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "menu.hpp"
#include "webservException.hpp"

void Menu::validateClosingToken(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken) {
	for (size_t i = 0; i < requiredTokenNumber; ++i) {
		if (this->getTokens()[i].find("{;}") != std::string::npos && this->getTokens()[i] != closingToken) {
			throw WebservException(std::string("Webserv: configuration file: unexpected '") += this->getTokens()[i] += "' in '" + directive += "' directive. Directive must be terminated by '" + closingToken += "'\n");
		}
	}
	if (this->getTokens()[requiredTokenNumber - 1] != closingToken) {
		throw WebservException("Webserv: configuration file: directive '" + directive += "' must be terminated by '" + closingToken += "'\n");
	}
}

void Menu::validateArgumentNumber(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken) {
	if (this->getTokens().size() < requiredTokenNumber) {
		throw WebservException("Webserv: configuration file: invalid number of arguments in '" + directive += "' directive\n");
	}
	for (size_t i = 0; i < requiredTokenNumber - 1; ++i) {
		if (this->getTokens()[i] == closingToken) {
			throw WebservException("Webserv: configuration file: invalid number of arguments in '" + directive += "' directive\n");
		}
	}
}

void Menu::validateDirectiveSyntax(std::string const& directive, size_t requiredTokenNumber, std::string const& closingToken) {
	validateArgumentNumber(directive, requiredTokenNumber, closingToken);
	validateClosingToken(directive, requiredTokenNumber, closingToken);
}