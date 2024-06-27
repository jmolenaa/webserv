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
	: FdHandler(cust.restaurant), status(stat), order(ord), recipe(rec), customer(cust), done(false),
	_buffer(""), _pipeFDs{-1, -1}, _CGI(nullptr), _fdOfFileToRead(-1), _doneReading(false), _fileSize(0)
{
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
//	if (done)
//		customer.eat();
}

void	Dish::input(int eventFD)
{
	(void)eventFD;
	ssize_t count = read(this->_inFD, _buffer, BUF_LIMIT - 1);
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
	else if (count == 0) {
		this->_removeHandler(this->_inFD);
//		this->_removeHandler(this->_outFD);
//		close(this->_outFD);
//		this->_outFD = -1;
		this->customer.eat();
	}
	else
	{
		Log::getInstance().print("Dish " + std::to_string(this->_fdOfFileToRead) + " is being cooked with " + std::to_string(count) + " ingredients");
		body.append(_buffer, count);
		if (body.size() == (size_t)this->_fileSize) {
			Log::getInstance().print("Dish " + std::to_string(this->_fdOfFileToRead) + " is ready to be eaten");
			this->_removeHandler(this->_inFD);
			this->customer.eat();
		}
	}
}

void Dish::_writeToPipe(ssize_t count) {
	ssize_t  bitesWritten = write(this->_outFD, _buffer, count);
	if (bitesWritten < 0) {
		Log::getInstance().printErr("Write error in dish input!\n");
		if (this->status.getState() != OK) {
			this->status.updateState(COUNT);
		}
		else {
			this->status.updateState(INTERNALERR);
		}
		doError();
	}
	else {
		this->_fileSize += bitesWritten;
	}
}

void	Dish::output(int eventFD)
{
	(void)eventFD;
	ssize_t	count = read(this->_fdOfFileToRead, _buffer, BUF_LIMIT - 1);
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
		Log::getInstance().print("Finished adding ingredients to dish " + std::to_string(this->_fdOfFileToRead) + "!\n");
		this->_doneReading = true;
		_removeHandler(this->_outFD);
//		_removeWithoutClosing(this->_outFD);
	}
	else
	{
		Log::getInstance().print(std::to_string(count) + " ingredients are being piped to dish " + std::to_string(this->_fdOfFileToRead) + "!");
		_writeToPipe(count);
//		write(this->_outFD, _buffer, count);
//		if (count < BUF_LIMIT - 1) {
//			_finishWriting();
//			Log::getInstance().print("Finished adding ingredients to dish " + std::to_string(this->_fdOfFileToRead) + "!\n");
//		}
	}
}

std::string	Dish::getDish()
{
	_generateHeader();
	return (header + body);
}

