/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _header.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:08:01 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/03 16:44:19 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include <ctime>
#include <sstream>

std::string	Response::_getDateTime()
{
    std::time_t now = std::time(nullptr);
    std::tm* now_tm = std::localtime(&now);
    char buffer[100];
    std::strftime(buffer, 100, "%a %d %b %Y %T %Z", now_tm);
    return std::string(buffer);
}

std::string	Response::_getType()
{
	if (_filetype == "html")
		return ("text/html");
	if (_filetype == "NONE")
		return (_filetype);
	else
		return("CGI stuff I guess");
}

void	Response::_generateHeader()
{
	_header += "HTTP/1.1 " + std::to_string(_status.getStatNum()) + " " + _status.getStatMessage() + "\r\n";
	// _body += "Date: " + _getDateTime() + "\r\n";
	_header += "Content-Type: " + _getType() + "\r\n";
	_header += "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	_header += "Connection: Closed\r\n";
}