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
#include <sstream>

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

void Menu::validateErrorCode(std::string const& errorCode) {
	// loops through error codes, if the given error code is not in our array it means it's an invalid code so we stop
	for (std::string const& validErrorCode : this->_errorCodesArray) {
		if (validErrorCode == errorCode) {
			return ;
		}
	}
	throw WebservException("Webserv: configuration file: invalid error number '" + errorCode + "' in 'return' directive\n");
}

void Menu::validateClientBodySize(std::stringstream const& streamBodySize) {
	// error checks, first one checks if the number given is outside the size_t range (sstream will set bit to fail)
	// second just checks for anything that isn't a number
	if (streamBodySize.fail() || streamBodySize.str().find_first_not_of("0123456789") != std::string::npos) {
		throw WebservException("Webserv: configuration file: invalid value '" + streamBodySize.str() + "' in 'client_max_body_size' directive\n");
	}
}

void Menu::validateMethod(const std::string &method, short methodBit, short allowedMethodsBits) {
	if (method != "GET" && method != "DELETE" && method != "POST") {
		throw WebservException("Webserv: configuration file: unrecognized method '" + method + "' in 'allowed_methods' directive\n");
	}
	// checks if method has already bene added
	if ((methodBit & allowedMethodsBits) != 0) {
		throw WebservException("Webserv: configuration file: method '" + method + "' used more than once in 'allowed_methods' directive\n");
	}
}

void Menu::validateRedirectCode(const std::string &redirectCode) {
	// loops through error codes, if the given error code is not in our array it means it's an invalid code so we stop
	// since this is for redirect codes, the only valid codes are the ones starting with 3 (all redirect codes are in between 300 and 400)
	for (std::string const& validErrorCode : this->_errorCodesArray) {
		if (validErrorCode == redirectCode && redirectCode[0] == '3') {
			return ;
		}
	}
	throw WebservException("Webserv: configuration file: invalid redirect code '" + redirectCode + "' in 'return' directive\n");
}
