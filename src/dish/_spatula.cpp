/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _spatula.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/14 18:12:24 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "dish.hpp"
#include "restaurant.hpp"
#include "webservException.hpp"

void Dish::_doMethod()
{
	method m = _order.getMethod();
	if ((m & _recipe.allowedMethods) == 0)
		return(_order.status.updateState(METHODNOTALLOWED));
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

void	Dish::_generateHeader()
{
	_header += "HTTP/1.1 " + std::to_string(_order.status.getStatNum()) + " " + _order.status.getStatMessage() + "\r\n";
	_header += "Content-Type: text/html \r\n";
	_header += "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	_header += "Connection: Closed\r\n";
}
