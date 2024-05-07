/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webservException.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/07 10:52:47 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/05/07 10:52:47 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

//
// Created by jmolenaa on 7-5-24.
//

#ifndef WEBSERV_WEBSERVEXCEPTION_HPP
#define WEBSERV_WEBSERVEXCEPTION_HPP

#include <exception>
#include <string>

class WebservException : public std::exception {

public:
	explicit WebservException(std::string args);
	char const*	what() const noexcept override { return _errorStr.c_str(); };

private:

	std::string	_errorStr;
};


#endif //WEBSERV_WEBSERVEXCEPTION_HPP
