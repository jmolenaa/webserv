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
#include "customer.hpp"

Dish::Dish(Status& stat, Order const& ord, Recipe rec, Customer& cust)
	: FdHandler(cust.restaurant), status(stat), order(ord), recipe(rec), customer(cust), done(false)
{
	_CGI = nullptr;
	if (order.getPath().find_last_of('/') == std::string::npos) {
		status.updateState(BAD);
	}
}

Dish::~Dish()
{
	_trashDish();
	delete _CGI;
}

void Dish::doMethod()
{
	if (this->status.getState() != OK) {
		doError();
	}
	method m = this->order.getMethod();
	if ((m & this->recipe.allowedMethods) == 0)
		return(this->status.updateState(METHODNOTALLOWED));
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
			this->status.updateState(METHODNOTALLOWED);
			return;
	}
	if (this->status.getState() != OK) {
		doError();
	}
	if (done)
		customer.eat();
}

void	Dish::input(int eventFD)
{
	(void)eventFD;
	ssize_t	count = read(_inFD, _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		Log::getInstance().printErr("Read error in dish input!\n");

		if (this->status.getState() != OK) {
			this->status.updateState(COUNT);
		}
		else {	
			this->status.updateState(INTERNALERR);
		}
		doError();
	}
	else if (count == 0)
	{
		Log::getInstance().print("Finished adding ingredients to dish " + std::to_string(_inFD) + "!\n");

		close(_inFD);
		close(_pipeFDs[1]);

		done = true;
		return;
	}
	else
	{
		Log::getInstance().print(std::to_string(count) + " ingredients are being piped to dish " + std::to_string(_inFD) + "!");
		write(_pipeFDs[1], _buffer, count);
	}
}

void	Dish::output(int eventFD)
{
	(void)eventFD;
	ssize_t count = read(_pipeFDs[0], _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		Log::getInstance().printErr("Read error in Dish output!\n");
		if (this->status.getState() != OK) {
			this->status.updateState(COUNT);
		}
		else {	
			this->status.updateState(INTERNALERR);
		}
		doError();
	}
	else
	{
		Log::getInstance().print("Dish " + std::to_string(_inFD) + " is being cooked with " + std::to_string(count) + " ingredients");
		_buffer[count] = '\0';
		body += std::string(_buffer);
	}
}

std::string	Dish::getDish()
{
	_generateHeader();
	return (header + body);
}

