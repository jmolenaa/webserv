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

Lexer::Lexer(std::string const& filename) : _file(filename) {

	if (_file.fail()) {
		throw WebservException("Failed to open configuration file: " + filename + "\n");
	}
}

Lexer::~Lexer() {
	this->_file.close();
}

