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
#include "waiter.hpp"
#include "webservException.hpp"

Dish::Dish(Order& order, Recipe& recipe, void* restuarantPointer)
	: FdHandler(restuarantPointer), _order(order), _recipe(recipe)
{
	if (_order.getPath().find_last_of('/') == std::string::npos)
		_order.status.updateState(BAD);
	else
	{
		method m = _order.getMethod();

		if ((m & recipe.allowedMethods) == 0)
			order.status.updateState(METHODNOTALLOWED);
		else
			_doMethod(m);
	}
	if (order.status.getState() != OK)
	{
		std::string errfile = _recipe.errorPaths[order.status.getState()];
		close(_outFD);
		_outFD = open(errfile.c_str(), O_RDONLY);
	}
	_dishToBody();
	_generateHeader();
}

Dish::~Dish(){}

std::string Dish::tmpGetResponse()
{
	return (_header + _body);
}

void	Dish::input(int eventFD)
{
	if (eventFD != _inFD)
   		throw WebservException("Bad call in Dish input\n");
}

void	Dish::output(int eventFD)
{
	if (eventFD != _inFD)
   		throw WebservException("Bad call in Dish output\n");
}