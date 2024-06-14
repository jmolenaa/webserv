/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _spatula.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/14 16:41:07 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "dish.hpp"
#include "restaurant.hpp"
#include "webservException.hpp"

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
			_order.status.updateState(METHODNOTALLOWED);
			return;
	}
}

/**
 * @todo needs to go through epoll
 * read BUF_LIMIT at a time until done.
*/
void Dish::_readyDish()
{
	_body = "\r\n";
	if (_inFD == -1)
	{
		_order.status.updateState(INTERNALERR);
		_body = "500 Internal Server Error";
	}
	else
	{
		Restaurant* restaurant = (Restaurant*)_order.resP;
		restaurant->addFdHandler(_inFD, this, EPOLLIN);
	}
}

void	Dish::_generateHeader()
{
	_header += "HTTP/1.1 " + std::to_string(_order.status.getStatNum()) + " " + _order.status.getStatMessage() + "\r\n";
	_header += "Content-Type: text/html \r\n";
	_header += "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	_header += "Connection: Closed\r\n";
}
