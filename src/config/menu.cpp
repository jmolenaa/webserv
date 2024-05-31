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

//config stuff goes here

//TODO
//open file, handle errors, like wrong filename or other crap
//handle syntax errors in the lexing, maybe we do that later
//go through node list and add things to server vector, then to the corresponding things in the class
//figure out a way to handle the server vector

// two options, make more states, saying we're in an actual directive state or we use deque and actualy go through the tokens to handle the directives one by one
// for the first we gonna need to rethink a bunch
// so we would have a directive state, depending on directive we would need 3 or 4 tokens, the last being the ; token
// so dependent on directive we would need to keep track of how many tokens we've done so far for that directive in particular and what directive we're doing
// other option is work with a deque and pop the tokens from the front to see what we're handling, idnetify what, then send it over to the correct function
// in this function which will be directive dependent we just handle the tokens we need to handle, popping them from the front eventually going back to the main loop, knowing the next thing is gonna be directive or state change
// this is probably the uglier not proper solution but it seems easier to handle

#include "menu.hpp"
#include "webservException.hpp"
#include "lexer.hpp"
#include <iostream>
#include <unordered_map>

Menu::Menu(std::string lol) : _currentState(NO_STATE) {
	(void)lol;
}

//Menu::~Menu() {
//}

Menu::state Menu::getState() const {
	return this->_currentState;
}

std::deque<std::string>& Menu::getTokens() {
	return this->_tokens;
}

std::vector<Cook> &Menu::getSettings() {
	return this->_serverSettings;
}

void Menu::setTokens(std::deque<std::string> tokens) {
	this->_tokens = std::move(tokens);
}

void Menu::setState(Menu::state newState) {
	this->_currentState = newState;
}

void Menu::lex(std::string const& filename) {

	Lexer	lexer(filename);

	this->setTokens(lexer.lex());
}

void	setupMap(std::unordered_map<std::string, std::function<void(Menu&)>>& runDirective) {
	runDirective["server"] = &Menu::serverDirective;
	runDirective["server_name"] = &Menu::serverNameDirective;
	runDirective["listen"] = &Menu::listenDirective;
	runDirective["location"] = &Menu::locationDirective;
	runDirective["index"] = &Menu::indexDirective;
	runDirective["error_page"] = &Menu::errorPageDirective;
	runDirective["autoindex"] = &Menu::autoindexDirective;
	runDirective["client_max_body_size"] = &Menu::clientMaxBodySizeDirective;
	runDirective["return"] = &Menu::returnDirective;
	runDirective["root"] = &Menu::rootDirective;
	runDirective["allowed_methods"] = &Menu::allowedMethodsDirective;
	runDirective["}"] = &Menu::closeBracketDirective;
}

bool Menu::isDirectiveInRightContext(std::string const& directive) const{
	if (this->getState() == NO_STATE && directive != "server") {
		return false;
	}
	else if (this->getState() == SERVER_STATE && directive == "server") {
		return false;
	}
	else if (this->getState() == LOCATION_STATE &&
			(directive == "server" || directive == "location" || directive == "listen" || directive == "server_name")) {
		return false;
	}
	return true;
}

void Menu::parse() {

	std::unordered_map<std::string, std::function<void(Menu&)>>	directiveFunctions;

	setupMap(directiveFunctions);

	while (!this->getTokens().empty()) {
		std::string directive = this->getTokens().front();
		this->getTokens().pop_front();
		if (directiveFunctions.count(directive) == 0) {
			throw WebservException("Webserv: configuration file: unrecognized directive: '" + directive + "'\n");
		}
		else if (!isDirectiveInRightContext(directive)) {
			throw WebservException("Webserv: configuration file: directive '" + directive + "' is in wrong context\n");
		}
		directiveFunctions[directive](*this);

		// this pops the ";" or "{" token from the directives, except from "}" which only changes the state of the parser
		if (directive != "}") {
			this->getTokens().pop_front();
		}
	}

}
