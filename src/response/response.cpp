/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   response.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:11:53 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/24 14:35:52 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include "helpers.hpp"

Response::Response(Epoll& epoll, Request& request, Location location) :
	_epoll(epoll), _location(location), _path(request.getPath()), _body("\r\n"), _filetype(NONE)
{
	if (_path.empty())
		_status.updateState(BAD);
	else
	{
		method m = request.getMethod();

		if ((m & location.allowedMethods) == 0)
			_status.updateState(METHODNOTALLOWED);

		_doMethod(m, request);
	}
	if (_status.getState() != OK)
		_getError();
	_generateHeader();
}

std::string Response::getResponseMessage()
{
	return (_header + _body);
}

