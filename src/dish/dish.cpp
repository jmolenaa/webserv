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
	_epoll(epoll), _order(order), _recipe(recipe), _dishFD(-1)
{
	if (_order.getPath().find_last_of('/') == std::string::npos)
		_status.updateState(BAD);
	else
	{
		method m = order.getMethod();

		if ((m & recipe.allowedMethods) == 0)
			_status.updateState(METHODNOTALLOWED);
		else
			_doMethod(m);
	}
	if (_status.getState() != OK)
	{
		std::string errfile = _recipe.errorPaths[_status.getState()];
		close(_dishFD);
		_dishFD = open(errfile.c_str(), O_RDONLY);
	}
	_dishToBody();
	_generateHeader();
}

std::string Dish::getMeal()
{
	return (_header + _body);
}

