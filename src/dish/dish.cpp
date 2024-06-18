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
	else {
		_doMethod(); //fix this so contents of html files get written to the read end of a pipe possibly using dup2
	}
	if (order.status.getState() != OK || _inFD < 0)	{
		_doError();
	}
	else
	{
		Log::getInstance().print("Readying dish " + std::to_string(_inFD) + " for order " + std::to_string(_order.getIn()));

		Restaurant* restaurant = (Restaurant*)_order.resP;
		restaurant->addFdHandler(_inFD, this, EPOLLIN);
	
		Log::getInstance().print("TESTING");
		char testbuffer[BUF_LIMIT];
		size_t count = read(_inFD, testbuffer, 50);
		testbuffer[count] = '\0';
		Log::getInstance().print("Test read: " + std::string(testbuffer));
	}
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
		_doError();
		_serveDish();
	}
	else
	{
		Log::getInstance().print("Dish is cooking!");
		_buffer[count] = '\0';
		_body += std::string(_buffer);
		if (count < BUF_LIMIT - 1)
		{
			// Restaurant* restaurant = (Restaurant*)_order.resP;
			// restaurant->removeFdHander(_inFD);
			close(_inFD);
			_serveDish();
		}
	}
}

void	Dish::_serveDish()
{
	_generateHeader();
	_sendMessage = _header + _body;
	_sendSize = _sendMessage.size();
	_sendPos = 0;
	_outFD = _order.getIn();
	Log::getInstance().print("Serving dish " + std::to_string(_inFD) + " to order " + std::to_string(_outFD));
	Restaurant* restaurant = (Restaurant*)_order.resP;
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