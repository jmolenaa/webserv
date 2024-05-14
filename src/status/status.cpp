/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   status.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 14:27:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/08 10:20:33 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "status.hpp"

Status::Status() : _state(OK)
{
	_status[OK].first = "OK";
	_status[OK].second = 200;

	_status[BAD].first = "Bad Request";
	_status[BAD].second = 400;

	_status[FORBIDDEN].first = "Forbidden";
	_status[FORBIDDEN].second = 403;

	_status[NOTFOUND].first = "Not Found";
	_status[NOTFOUND].second = 404;

	_status[METHODNOTALLOWED].first = "Method Not Allowed";
	_status[METHODNOTALLOWED].second = 405;

	_status[LENGTHREQUIRED].first = "Length Required";
	_status[LENGTHREQUIRED].second = 411;

	_status[TOOLARGE].first = "Payload Too Large";
	_status[TOOLARGE].second = 413;

	_status[UNSUPPORTED].first = "Unsupported Media Type";
	_status[UNSUPPORTED].second = 415;

	_status[INTERNALERR].first = "Internal Server Error";
	_status[INTERNALERR].second = 500;
}

std::string& Status::getStatMessage()
{
	return (_status[_state].first);
}

short& Status::getStatNum()
{
	return (_status[_state].second);
}

void Status::updateState(key state)
{
	_state = state;
}

Status::key Status::getState()
{
	return _state;
}