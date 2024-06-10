/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: janmolenaar <janmolenaar@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/29 13:34:15 by janmolenaar   #+#    #+#                 */
/*   Updated: 2024/04/29 13:34:15 by janmolenaar   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Jan Molenaar on 29/04/2024.
//

#include "webservException.hpp"
#include "lexer.hpp"
#include <iostream>

Lexer::Lexer(std::string const& filename) : _file(filename) {
	if (filename.substr(filename.length() - 5, 5) != ".conf") {
		throw WebservException("Wrong file extension: " + filename + ", file should end with '.conf'\n");
	}
	if (this->_file.fail()) {
		throw WebservException("Failed to open configuration file: " + filename + "\n");
	}
}

Lexer::~Lexer() {
	this->_file.close();
}

std::deque<std::string> Lexer::lex() {

	std::deque<std::string>	tokens;
	std::string				buffer;

	while (std::getline(this->_file, buffer)) {

		size_t	endPosition = 0;
		size_t	startPosition = 0;

		while (endPosition != std::string::npos){
			// skipping whitespaces
			startPosition = buffer.find_first_not_of(" \t", endPosition);
			if (startPosition == std::string::npos) {
				break ;
			}

			// finding first character that's a delimiter
			endPosition = buffer.find_first_of(" \t\n;{}", startPosition);

			// this happens when we encounter a {,} or ;, we want to make a token out of
			// that delimiter, so we increase endPosition by 1
			if (startPosition == endPosition) {
				++endPosition;
			}

			tokens.push_back(buffer.substr(startPosition, endPosition - startPosition));
		}
	}
	return tokens;
}
