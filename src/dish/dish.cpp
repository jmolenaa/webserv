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
		close(_inFD);
		_inFD = open(errfile.c_str(), O_RDONLY);
	}
	_readyDish();
}

Dish::~Dish(){}

void	Dish::input(int eventFD)
{
	if (eventFD != _inFD)
   		throw WebservException("Bad call in Dish input\n");
			

	ssize_t	count = read(_inFD, _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		Log::getInstance().print("Count < 0 in Dish read!");
		_order.status.updateState(INTERNALERR);
		Restaurant* restaurant = (Restaurant*)_order.resP;
		restaurant->removeFdHander(_inFD);
		close(_inFD);
		_serveDish();
	}
	else
	{
		_buffer[count] = '\0';
		_body += std::string(_buffer);
		if (count < BUF_LIMIT - 1)
			_serveDish();
	}
}

void	Dish::_serveDish()
{
	Restaurant* restaurant = (Restaurant*)_order.resP;
	restaurant->removeFdHander(_inFD);
	close(_inFD);
	_generateHeader();
	_sendMessage = _header + _body;
	_sendSize = _sendMessage.size();
	_sendPos = 0;
	_outFD = _order.getIn();
	restaurant->addFdHandler(_outFD, this, EPOLLOUT);
}

void	Dish::output(int eventFD)
{
	if (eventFD != _outFD)
   		throw WebservException("Bad call in Dish output\n");
	
	if (_sendSize <= 0)	{
		return (_order.orderDone());
	}
	const char* response = _sendMessage.c_str();
	response += _sendPos;
	size_t	size = std::strlen(response);
	if (size > BUF_LIMIT)
		size = BUF_LIMIT;
	_sendPos += size;
	_sendSize -= send(_outFD, response, size, 0);
}