/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   config.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: janmolenaar <janmolenaar@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/29 10:35:05 by janmolenaar   #+#    #+#                 */
/*   Updated: 2024/04/29 10:35:05 by janmolenaar   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//config stuff goes here

#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <string>
#include <vector>
#include "server.hpp"
#include "lexer.hpp"

class ConfigParser {

public:

	ConfigParser() = default;

	void				lex(std::string const& filename);
	void				parse();

private:


};

#endif
