/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer.hpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: janmolenaar <janmolenaar@student.codam.nl>   +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/29 13:34:13 by janmolenaar   #+#    #+#                 */
/*   Updated: 2024/04/29 13:34:13 by janmolenaar   ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//
// Created by Jan Molenaar on 29/04/2024.
//

#ifndef WEBSERV_LEXER_HPP
#define WEBSERV_LEXER_HPP

//#include <vector>
#include <fstream>

class Lexer {

public:

	explicit Lexer(std::string const& filename);
	~Lexer();

private:

	std::ifstream	_file;

//	class Node {
//	public:
//	};

};


#endif //WEBSERV_LEXER_HPP
