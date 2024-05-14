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

#include <vector>
#include <fstream>

class Lexer {

public:

	Lexer() = default;
	explicit Lexer(std::string const& filename);
	~Lexer();

	void	lex();
	std::vector<std::string>	getTokens() const;

private:

	std::ifstream				_file;
	std::vector<std::string>	_tokens;

};

std::ostream&	operator<<(std::ostream& out, Lexer const& rhs);

#endif //WEBSERV_LEXER_HPP
