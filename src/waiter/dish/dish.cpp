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
#include <cstring>

#include "dish.hpp"
#include "order.hpp"
#include "waiter.hpp"
#include "restaurant.hpp"
#include "webservException.hpp"
#include "customer.hpp"

Dish::Dish(Status& status, Order const& order, Recipe recipe, void* restuarantPointer, void* customerPointer)
	: FdHandler(restuarantPointer), _status(status), _order(order), _recipe(recipe), _customerPointer(customerPointer)
{
	if (_order.getPath().find_last_of('/') == std::string::npos)
		_status.updateState(BAD);
}

Dish::~Dish()
{
	_trashDish();
}

void Dish::doMethod()
{
	if (_status.getState() != OK) {
		_doError();
	}
	method m = _order.getMethod();
	if ((m & _recipe.allowedMethods) == 0)
		return(_status.updateState(METHODNOTALLOWED));
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
	if (_status.getState() != OK) {
		_doError();
	}
}

void	Dish::input(int eventFD)
{
	(void)eventFD;
	ssize_t	count = read(_inFD, _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		Log::getInstance().printErr("SOMETHING WHENT WRONG IN DISH INPUT\n");

		if (_status.getState() != OK) {
			_status.updateState(COUNT);
		}
		else {	
			_status.updateState(INTERNALERR);
		}
		_doError();
	}
	else if (count == 0)
	{
		Log::getInstance().print("Finished adding ingredientes from " + std::to_string(_inFD) + "!\n");

		// close(_inFD);
		// close(_pipeFDs[1]);

		Customer* myCustomer = (Customer*)_customerPointer;
		myCustomer->eat();
	}
	else
	{
		Log::getInstance().print(std::to_string(count) + " ingredients are being added!");
		write(_pipeFDs[1], _buffer, count);
	}
}

void	Dish::output(int eventFD)
{
	(void)eventFD;
	ssize_t count = read(_pipeFDs[0], _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		Log::getInstance().printErr("SOMETHING WHENT WRONG\n");
		if (_status.getState() != OK) {
			_status.updateState(COUNT);
		}
		else {	
			_status.updateState(INTERNALERR);
		}
		_doError();
	}
	else
	{
		Log::getInstance().print("Dish is cooking " + std::to_string(count) + " ingredients");
		_buffer[count] = '\0';
		_body += std::string(_buffer);
	}
}

std::string	Dish::getDish()
{
	_generateHeader();
	return (_header + _body);
}

