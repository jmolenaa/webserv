/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _spatula.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/12 14:58:22 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

void Dish::_doMethod(method m)
{
	switch (m)
	{
		case (GET):
			_get();
			break;
		case (POST):
			_post();
			break;
		case (DELETE):
			_delete();
			break;
		default:
			_status.updateState(METHODNOTALLOWED);
			return;
	}
}

/**
 * @todo needs to go through epoll
 * read BUF_LIMIT at a time until done.
*/
void Dish::_dishToBody()
{
	_body = "\r\n";
	if (_dishFD == -1)
	{
		_body += "INTERNAL ERROR";
		_status.updateState(INTERNALERR);
	}
	
	char 	buffer[BUF_LIMIT] = "";
	ssize_t	count = read(_dishFD, buffer, BUF_LIMIT - 1);
	while (count)
	{
		buffer[count] = '\0';
		if (count < 0)
		{
			_status.updateState(INTERNALERR);
			break;
		}
		std::string append(buffer);
		_body += append.substr(0, count);
		count = read(_dishFD, buffer, BUF_LIMIT - 1);
	}
	close(_dishFD);
}

void	Dish::_generateHeader()
{
	_header += "HTTP/1.1 " + std::to_string(_status.getStatNum()) + " " + _status.getStatMessage() + "\r\n";
	_header += "Content-Type: text/html \r\n";
	_header += "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	_header += "Connection: Closed\r\n";
}
