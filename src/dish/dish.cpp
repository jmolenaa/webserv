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

#include <unistd.h>

#include "dish.hpp"
#include "order.hpp"
#include "webservException.hpp"

Dish::Dish(Status& stat, void* order, Recipe& recipe) :
	_status(stat), _order(order), _recipe(recipe), _dishFD(-1)
{
	Order* ord = (Order*)_order;
	if (ord->getPath().find_last_of('/') == std::string::npos)
		_status.updateState(BAD);
	else
	{
		method m = ord->getMethod();

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

Dish::~Dish(){}

std::string Dish::tmpGetResponse()
{
	return (_header + _body);
}

status	Dish::input(int eventFD)
{
	if (eventFD != _dishFD)
   		throw WebservException("Bad call in Dish input\n");
	return (OK);
}

status	Dish::output(int eventFD)
{
	if (eventFD != _dishFD)
   		throw WebservException("Bad call in Dish output\n");
	return (OK);
}