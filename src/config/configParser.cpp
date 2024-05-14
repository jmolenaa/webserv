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
//get lexing going
//open file, handle errors, like wrong filename or other crap
//handle syntax errors in the lexing, maybe we do that later
//go through node list and add things to server vector, then to the corresponding things in the class
//figure out a way to handle the server vector

#include "configParser.hpp"
#include "webservException.hpp"

ConfigParser::ConfigParser(std::string const& filename) : _lexer(filename) {
}

//ConfigParser::~ConfigParser() {
//}

void ConfigParser::lex() {

	this->_lexer.lex();

}

void ConfigParser::parse() {

//	Server	newServer;
//	std::vector<Server>	servers({Server()});
//	return servers;
}
