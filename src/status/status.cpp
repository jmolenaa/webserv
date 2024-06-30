/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   status.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 14:27:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/24 13:18:50 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "status.hpp"

Status::Status() : _state(OK)
{
	_status[OK].first = "OK";
	_status[OK].second = 200;

	_status[MOVEDPERMANENTLY].first = "Moved permanently";
	_status[MOVEDPERMANENTLY].second = 301;

	_status[FOUND].first = "Found";
	_status[FOUND].second = 302;

	_status[TEMPORARYREDIRECT].first = "Temporary redirect";
	_status[TEMPORARYREDIRECT].second = 307;

	_status[PERMANENTREDIRECT].first = "Permanent redirect";
	_status[PERMANENTREDIRECT].second = 308;

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

	_status[LOOPDETECTED].first = "Loop Detected";
	_status[LOOPDETECTED].second = 508;
}

std::string& Status::getStatMessage()
{
	return (_status[_state].first);
}

short& Status::getStatNum()
{
	return (_status[_state].second);
}

void Status::updateState(status state)
{
	_state = state;
}

status Status::getState()
{
	return _state;
}