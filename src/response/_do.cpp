/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _do.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/24 12:25:56 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

void Response::_doMethod(method meth, Request& request)
{
	switch (meth)
	{
		case (GET):
			_get();
			break;
		case (POST):
			_post(request);
			break;
		case (DELETE):
			_delete(request);
			break;
		default:
			_status.updateState(METHODNOTALLOWED);
			break;
	}
}

void Response::_post(Request& request)
{
	std::string body = request.getBody();
	_body += "You tried to post: '" + body + "'\nWell, nice try I guess.";
}

void Response::_delete(Request& request)
{
	std::string path = request.getPath();
	_body += "You tried to delete: '" + path + "'\nNice try but deleting is not yet implemented.";
}

void Response::_executeCGI()
{
	_body += "CGI STUFF NOT YET IMPLEMENTED";
}