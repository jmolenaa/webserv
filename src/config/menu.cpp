/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: janmolenaar <janmolenaar@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/29 10:35:07 by janmolenaar   #+#    #+#                 */
/*   Updated: 2024/04/29 10:35:07 by janmolenaar   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "menu.hpp"
#include "webservException.hpp"
#include "lexer.hpp"
#include <unordered_map>
#include "status.hpp"

Menu::Menu(std::string const& filename) : _currentState(NO_STATE) , _currentCook(nullptr), _currentRecipe(nullptr){
	initErrorCodes();
	Lexer	lexer(filename);
	this->setTokens(lexer.lex());
	this->parse();
}

void Menu::initErrorCodes() {
	Status	statusCodes;

	for (size_t i = 0; i < COUNT; ++i) {
		this->_errorCodesArray[i] = std::to_string(statusCodes.getStatNum());
		statusCodes.updateState((status)(i + 1));
	}
}

Menu::~Menu() {
	delete getCurrentRecipe();
	delete getCurrentCook();
}

Menu::state Menu::getState() const {
	return this->_currentState;
}

std::deque<std::string>& Menu::getTokens() {
	return this->_tokens;
}

std::vector<Kitchen> &Menu::getKitchens() {
	return this->_kitchens;
}

Cook *Menu::getCurrentCook() {
	return this->_currentCook;
}

Recipe *Menu::getCurrentRecipe() {
	return this->_currentRecipe;
}

void Menu::setCurrentCook(Cook *newCook) {
	delete this->getCurrentCook();
	this->_currentCook = newCook;
}

void Menu::setCurrentRecipe(Recipe *newRecipe) {
	delete this->getCurrentRecipe();
	this->_currentRecipe = newRecipe;
}

void Menu::setTokens(std::deque<std::string> tokens) {
	this->_tokens = std::move(tokens);
}

void Menu::setState(Menu::state newState) {
	this->_currentState = newState;
}

void	setupMap(directiveFunctions& runDirective) {
	runDirective["server"] = &Menu::serverDirective;
	runDirective["server_name"] = &Menu::serverNameDirective;
	runDirective["listen"] = &Menu::listenDirective;
	runDirective["location"] = &Menu::locationDirective;
	runDirective["index"] = &Menu::indexDirective;
	runDirective["error_page"] = &Menu::errorPageDirective;
	runDirective["autoindex"] = &Menu::autoIndexDirective;
	runDirective["client_max_body_size"] = &Menu::clientMaxBodySizeDirective;
	runDirective["return"] = &Menu::returnDirective;
	runDirective["root"] = &Menu::rootDirective;
	runDirective["allowed_methods"] = &Menu::allowedMethodsDirective;
	runDirective["upload_dir"] = &Menu::uploadDirDirective;
	runDirective["allow_upload"] = &Menu::allowUploadDirective;
	runDirective["cgi_extension"] = &Menu::cgiExtensionDirective;
	runDirective["allow_cgi"] = &Menu::allowCgiDirective;
	runDirective["}"] = &Menu::closeBracketDirective;
}

void Menu::parse() {
	directiveFunctions	directiveFunctions;
	setupMap(directiveFunctions);

	while (!this->getTokens().empty()) {
		std::string directive = this->popFrontToken();
		this->validateDirective(directive, directiveFunctions);
		directiveFunctions[directive](*this);
		// this pops the ";" or "{" token from the directives, except from "}" which only changes the state of the parser
		if (directive != "}") {
			this->getTokens().pop_front();
		}
	}
}
