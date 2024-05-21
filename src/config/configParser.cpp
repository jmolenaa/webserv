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

#include "configParser.hpp"
#include "webservException.hpp"
#include <iostream>
#include <unordered_map>

ConfigParser::ConfigParser() : _currentState(NO_STATE) {
}

//ConfigParser::~ConfigParser() {
//}

ConfigParser::state ConfigParser::getState() const {
	return this->_currentState;
}

std::deque<std::string>& ConfigParser::getTokens() {
	return this->_tokens;
}

void ConfigParser::setTokens(std::deque<std::string> tokens) {
	this->_tokens = std::move(tokens);
}

void ConfigParser::lex(std::string const& filename) {

	Lexer	lexer(filename);

	this->setTokens(lexer.lex());
//	for (const auto & _token : this->_tokens) {
//		std::cout << _token << std::endl;
//	}
}

void ConfigParser::serverDirective() {
	std::cout << "Server directive\n";
	this->_currentState = SERVER_STATE;
}

void	setupMap(std::unordered_map<std::string, std::function<void(ConfigParser&)>>& runDirective) {
	runDirective["server"] = &ConfigParser::serverDirective;
}


void ConfigParser::parse() {

	std::unordered_map<std::string, std::function<void(ConfigParser&)>>	runDirective;

	setupMap(runDirective);

	while (!this->getTokens().empty()) {
		std::string token = this->getTokens().front();
		std::cout << this->getState() << std::endl;
		if (runDirective.count(token)) {
			runDirective[token](*this);
		}
		this->getTokens().pop_front();
//		std::cout << this->getTokens().size() << std::endl;
//		std::cout << "infinite\n" << std::endl;

	}

	//	for (std::string const& token : this->getTokens()) {
//
//		std::cout << token << "\n";
//	}
//	std::cout << this->_serverSettings.size() << std::endl;
//	for (const auto & _token : this->_tokens) {
//		std::cout << _token << std::endl;
//	}
//	Server	newServer;
//	std::vector<Server>	servers({Server()});
//	return servers;
}
