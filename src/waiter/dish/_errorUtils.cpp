/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _errorUtils.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 11:08:30 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/06/28 11:08:30 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"
#include "customer.hpp"

// TODO add max body size here
bool Dish::_thereIsAnInitialError() {
	if (this->status.getState() != OK) {
		return true;
	}
	if ((this->order.getMethod() & this->recipe.allowedMethods) == 0) {
		this->status.updateState(METHODNOTALLOWED);
	}
	if (this->order.getLength() > this->recipe.maxBodySize) {
		this->status.updateState(TOOLARGE);
	}
	if (this->order.getLength() == 0 && !this->order.getBody().empty()) {
		this->status.updateState(LENGTHREQUIRED);
	}
	else {
		return false;
	}
	return true;
}

void Dish::_horribleError() {
	status.updateState(INTERNALERR);
	body = "500 webserve encountered a critical internal error";
	customer.eat();
}

void	Dish::doError()
{
	_trashDish();

	if (status.getState() == COUNT) {
		_horribleError();
		return ;
	}
	std::string errFile = recipe.errorPaths[status.getState()];
	this->_fdOfFileToRead = open(errFile.c_str(), O_RDONLY);
	if (this->_fdOfFileToRead < 0)
	{
		_horribleError();
	}
	else
	{
		std::cout << "WE SHOULD BE IN HERE\n";
		_doPipe();
		if (status.getState() == COUNT) {
			_horribleError();
		}
	}
}

void Dish::_handleFileError(int errorCode) {
	if (errorCode == EACCES) {
		this->status.updateState(FORBIDDEN);
	}
	else {
		this->status.updateState(NOTFOUND);
	}
}

void Dish::_handleOutputError() {
	Log::getInstance().printErr("Write error in dish input!\n");
	if (this->status.getState() != OK) {
		this->status.updateState(COUNT);
	}
	else {
		this->status.updateState(INTERNALERR);
	}
	doError();
	// if the error we encountered isn't critical we attempt to read the error file
	// this is only necessary for writing to the pipe, due to how epoll handles pipe events
	// errors during writing to pipes, we want to setup a new one, but if that happens we, we close
	// the previous ones and overwrite the fd with new ones, since an event on the file descriptor
	// of the previous reading end might have bene triggered already we would go into input and block on read call
	// this way we go back to output and try to write again
	if (this->_fdOfFileToRead != -1) {
		this->output(this->_fdOfFileToRead);
	}
}
