/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   webservException.hpp                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: yizhang <yizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/08 15:41:49 by yizhang       #+#    #+#                 */
/*   Updated: 2024/05/16 14:03:17 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_WEBSERVEXCEPTION_HPP
#define WEBSERV_WEBSERVEXCEPTION_HPP

#include <exception>
#include <string>

class WebservException : public std::exception {

public:
	explicit WebservException(std::string args);
	char const*	what() const noexcept override { return _errorStr.c_str(); };

private:
    WebservException();
	std::string	_errorStr;
};


#endif //WEBSERV_WEBSERVEXCEPTION_HPP