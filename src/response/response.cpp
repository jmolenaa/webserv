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

Response::Response(Request& request) : _path(request.getPath()), _filetype(INDEX)
{
	if (request.getMethod() == GET)
		_get();
	else if (request.getMethod() == POST)
		_post();
	else if (request.getMethod() == DELETE)
		_delete();
	_generateHeader();
}

std::string Response::getResponseMessage()
{
	return (_header + _body);
}

