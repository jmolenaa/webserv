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

#include "menu.hpp"
#include <iostream>
#include "webservException.hpp"
#include <sstream>

void Menu::serverDirective() {

	this->validateDirectiveSyntax("server", 1, "{");
	this->setState(SERVER_STATE);
	this->setCurrentCook(new Cook());
	this->setCurrentRecipe(new Recipe());

}

void Menu::serverNameDirective() {

	this->validateDirectiveSyntax("server_name", 2, ";");
	this->getCurrentCook()->setName(this->popFrontToken());
}

// TODO
void Menu::listenDirective() {
	this->validateDirectiveSyntax("listen", 2, ";");
	std::string	IpAndPort = this->popFrontToken();

	// Initialising the strings to " ", so I have a way of identifying that they aren't part of the directive
	// this is because nullptr does not exist for std::string
	std::string	newIp = " ";
	std::string newPort = " ";
	splitAndInitialiseIpAndPortStrings(IpAndPort, newIp, newPort);
	std::cout << "'" << newIp << "'	port: '" << newPort << "'\n";


}

// TODO
void Menu::locationDirective() {
	this->validateDirectiveSyntax("location", 2, "{");

	std::cout << "location" << std::endl;
	this->getTokens().pop_front();
}

void Menu::indexDirective() {
	this->validateDirectiveSyntax("index", 2, ";");
	this->getCurrentRecipe()->index = this->popFrontToken();
}

// TODO
// talk about how to handle the paths to error_page, do I add root to the front of whatever they give or not
void Menu::errorPageDirective() {
	this->validateDirectiveSyntax("error_page", 3, ";");
	std::string	errorCode = this->popFrontToken();
 	this->validateErrorCode(errorCode);

	// identifies the index of the errorCode given in our errorPaths array
	// then adds the path given into the array on the correct index
	int	arrayIndex = this->getArrayIndex(errorCode);
	this->getCurrentRecipe()->errorPaths[arrayIndex] = this->popFrontToken();

}

void Menu::autoIndexDirective() {
	this->validateDirectiveSyntax("autoindex", 2, ";");
	std::string	autoIndexValue = this->popFrontToken();
	if (autoIndexValue != "off" && autoIndexValue != "on") {
		throw WebservException("Webserv: configuration file: invalid value '" + autoIndexValue + "' in 'autoindex' directive, should be 'on' or 'off'\n");
	}
	if (autoIndexValue == "off") {
		this->getCurrentRecipe()->autoindex = false;
	}
	else {
		this->getCurrentRecipe()->autoindex = true;
	}
}

void Menu::clientMaxBodySizeDirective() {
	this->validateDirectiveSyntax("client_max_body_size", 2, ";");

	//using stringStreams to easily convert the number to size_t
	std::stringstream	streamBodySize(this->popFrontToken());
	size_t				clientBodySizeInNbrForm;
	streamBodySize >> clientBodySizeInNbrForm;
	validateClientBodySize(streamBodySize);

	this->getCurrentRecipe()->maxBodySize = clientBodySizeInNbrForm;
}

void Menu::returnDirective() {
	this->validateDirectiveSyntax("return", 3, ";");
	std::string	redirectCode = this->popFrontToken();
	this->validateRedirectCode(redirectCode);

	// identifies the array index of the code given as redirection which corresponds to the
	// actual code in the status enum
	this->getCurrentRecipe()->redir.first = (status)getArrayIndex(redirectCode);
	this->getCurrentRecipe()->redir.second = this->popFrontToken();
}

void Menu::rootDirective() {
	this->validateDirectiveSyntax("root", 2, ";");
	this->getCurrentRecipe()->root = this->popFrontToken();
}

void Menu::allowedMethodsDirective() {
	this->validateDirectiveSyntax("allowed_methods", 2, ";");
	std::string	allowedMethodsString = this->popFrontToken();

	// If only NONE is in the directive, means no methods allowed, this only works if the directive is NONE
	if (allowedMethodsString == "NONE") {
		this->getCurrentRecipe()->allowedMethods = 0;
		return ;
	}
	short		allowedMethodsBits = 0;

	// loops over string, over all commas, identifies and validates the methods and adds them to one bitmask
	size_t	i = 0;
	for (size_t start = 0; i != std::string::npos; start = i + 1) {
		i = allowedMethodsString.find(',', start);
		std::string method = allowedMethodsString.substr(start, i - start);
		short		methodBit = identifyMethod(method);
		validateMethod(method, methodBit, allowedMethodsBits);
		allowedMethodsBits |= methodBit;
	}
	this->getCurrentRecipe()->allowedMethods = allowedMethodsBits;
}

// TODO
void Menu::closeBracketDirective() {

}

