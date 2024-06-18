/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _spatula.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/18 18:28:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "dish.hpp"
#include "restaurant.hpp"
#include "webservException.hpp"

void	Dish::_doPipe()
{
	int success = pipe(_pipeFDs);
	if (success < 0) {
		_status.updateState(INTERNALERR);
	}
	Restaurant* restaurant = (Restaurant*)resP;
	restaurant->addFdHandler(_pipeFDs[0], this, EPOLLIN);
	restaurant->addFdHandler(_pipeFDs[1], this, EPOLLOUT);	
}

void	Dish::_trashDish()
{
	Log::getInstance().print("Destroying dish");
	_body = "";
	close(_inFD);
	Restaurant* restaurant = (Restaurant*)resP;
	restaurant->removeFdHander(_pipeFDs[0]);
	restaurant->removeFdHander(_pipeFDs[1]);
	close(_pipeFDs[0]);
	close(_pipeFDs[1]);
}

void	Dish::_doError()
{
	_trashDish();

	std::string errfile = _recipe.errorPaths[_status.getState()];
	_inFD = open(errfile.c_str(), O_RDONLY);
	if (_inFD < 0 || _status.getState() == COUNT)
	{
		_status.updateState(INTERNALERR);
		_body = "500 webserve encountered a critical internal error";
		_generateHeader();
		std::string response = _header + _body;
		Customer* myCustomer = (Customer*)_customerPointer;
		myCustomer->eat();
	}
	else 
	{
		_doPipe();
		input(_inFD);
	}
}

//Make this happen on each send.
void	Dish::_generateHeader()
{
	_header += "HTTP/1.1 " + std::to_string(_status.getStatNum()) + " " + _status.getStatMessage() + "\r\n";
	_header += "Content-Type: text/html \r\n";
	_header += "Content-Length: " + std::to_string(_body.size()) + "\r\n";
	_header += "Connection: Closed\r\n\r\n";
}
