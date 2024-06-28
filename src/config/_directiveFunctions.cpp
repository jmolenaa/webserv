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

void Menu::listenDirective() {
	this->validateDirectiveSyntax("listen", 2, ";");
	std::string	ipAndPort = this->popFrontToken();
	std::string	newIp;
	std::string newPort;
	splitAndInitialiseIpAndPortStrings(ipAndPort, newIp, newPort);
	this->setNewIp(newIp, ipAndPort);
	this->setNewPort(newPort, ipAndPort);
}

void Menu::locationDirective() {
	this->validateDirectiveSyntax("location", 2, "{");
	this->setState(LOCATION_STATE);
	// if we hadn't encountered a location directive yet, we add the default recipe
	// to the cookbook
	if (this->getCurrentRecipe() != nullptr) {
		this->getCurrentCook()->addToCookbook(*this->getCurrentRecipe());
	}
	// create a new recipe based on the default recipe then assign the location page to it
	this->setCurrentRecipe(new Recipe(this->getCurrentCook()->getRecipe("root")));
	this->getCurrentRecipe()->page = this->popFrontToken();
	if (this->getCurrentRecipe()->page == "root") {
		throw WebservException("Webserv: configuration file: location directive contains the 'root' route\n");
	}
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
	setAndValidateBoolValue(this->popFrontToken(), this->getCurrentRecipe()->autoindex, "autoindex");
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
	if (this->getCurrentRecipe()->page != "root") {
		this->getCurrentRecipe()->overwrittenRoot = true;
	}
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

void Menu::allowUploadDirective() {
	this->validateDirectiveSyntax("allow_upload", 2, ";");
	setAndValidateBoolValue(this->popFrontToken(), this->getCurrentRecipe()->allowUploading, "allow_upload");
}

void Menu::uploadDirDirective() {
	this->validateDirectiveSyntax("upload_dir", 2, ";");
	this->getCurrentRecipe()->uploadDir = this->popFrontToken();
}

void Menu::cgiExtensionDirective() {
	this->validateDirectiveSyntax("cgi_extension", 2, ";");
	this->getCurrentRecipe()->cgiExtension = this->popFrontToken();
}

void Menu::allowCgiDirective() {
	this->validateDirectiveSyntax("allow_cgi", 2, ";");
	setAndValidateBoolValue(this->popFrontToken(), this->getCurrentRecipe()->allowCgi, "allow_cgi");
}

void Menu::closeBracketDirective() {
	if (this->getState() == LOCATION_STATE) {
		this->closeLocation();
	}
	else {
		this->closeServer();
	}
}

