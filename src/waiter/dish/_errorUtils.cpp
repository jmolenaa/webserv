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
		return true;
	}
	return false;
}

void Dish::_horribleError() {
	status.updateState(INTERNALERR);
	body = "500 webserve encountered a critical internal error";
	_generateHeader();
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
		_doPipe();
		if (status.getState() == COUNT) {
			_horribleError();
		}
	}
}

void Dish::_handleOpenError(int errorCode) {
	if (errorCode == EACCES) {
		this->status.updateState(FORBIDDEN);
	}
	else {
		this->status.updateState(NOTFOUND);
	}
}
