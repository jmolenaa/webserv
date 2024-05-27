/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   dish.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:11:53 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/24 14:35:52 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

Dish::Dish(Epoll& epoll, Order& order, Recipe& recipe) :
	_epoll(epoll), _location(recipe), _path(order.getPath()), _body("\r\n"), _filetype(NONE)
{
	if (_path.empty())
		_status.updateState(BAD);
	else
	{
		method m = order.getMethod();

		if ((m & recipe.allowedMethods) == 0)
			_status.updateState(METHODNOTALLOWED);

		_doMethod(m, order);
	}
	if (_status.getState() != OK)
		_getError();
	_generateHeader();
}

std::string Dish::getMeal()
{
	return (_header + _body);
}

