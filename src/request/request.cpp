/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:11:53 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/23 19:02:57 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request(char *request)
{
	_request = request;
	
	_reason = Helpers::_keyValueFind(_request, "Received: ", ' ');

	std::string	tmp;
	tmp = Helpers::_keyValueFind(_request, "Host: ", ' ');
	_hostname = Helpers::_keyValueFind(_request, "Host: ", ':');
	tmp = Helpers::_keyValueFind(tmp, ":", 0);
	if (!tmp.empty())
		_port = std::stoi(tmp);
	else
		_port = 0;
	
	tmp = Helpers::_keyValueFind(_request, "Content-Length: ", ' ');
	if (!tmp.empty())
		_contentLength = std::stoi(tmp);
	else
		_contentLength = 0;
	
	_body = _extractBody(_request);
}

std::string Request::_extractBody(std::string _request)
{
	return ("");
}