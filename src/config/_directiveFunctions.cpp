/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _directiveFunctions.cpp                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/17 11:50:26 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/17 11:50:26 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "configParser.hpp"
#include <iostream>
#include "webservException.hpp"

void ConfigParser::serverDirective() {
	validateDirectiveSyntax("server", 1, "{");
	std::cout << "server" << std::endl;
	this->setState(SERVER_STATE);
	this->getSettings().emplace_back();
}

void ConfigParser::serverNameDirective() {
	std::string	token;

	if (this->getTokens().empty()) {
		throw WebservException("Webserv: configuration file: encountered");
	}
	std::cout << "servername" << std::endl;
	this->getTokens().pop_front();
}

void ConfigParser::listenDirective() {
	std::cout << "listen" << std::endl;
	this->getTokens().pop_front();

}

void ConfigParser::locationDirective() {
	std::cout << "location" << std::endl;
	this->getTokens().pop_front();
}

void ConfigParser::indexDirective() {
	std::cout << "index" << std::endl;
	this->getTokens().pop_front();
}

void ConfigParser::errorPageDirective() {
	std::cout << "errorpage" << std::endl;
	this->getTokens().pop_front();
	this->getTokens().pop_front();

}

void ConfigParser::autoindexDirective() {
	std::cout << "autoindex" << std::endl;
	this->getTokens().pop_front();

}

void ConfigParser::clientMaxBodySizeDirective() {
	std::cout << "clinetmaxbody" << std::endl;
	this->getTokens().pop_front();

}

void ConfigParser::returnDirective() {
	std::cout << "return" << std::endl;
	this->getTokens().pop_front();
	this->getTokens().pop_front();

}

void ConfigParser::rootDirective() {
	std::cout << "root" << std::endl;
	this->getTokens().pop_front();

}

void ConfigParser::allowedMethodsDirective() {
	this->getTokens().pop_front();

}

void ConfigParser::closeBracketDirective() {

}

