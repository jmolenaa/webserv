/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _spatula.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/24 14:04:40 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

#include <unistd.h>
#include "restaurant.hpp"
#include "webservException.hpp"
#include "customer.hpp"

void	Dish::_doPipe()
{
	int success = pipe(_pipeFDs);
	if (success < 0) {
		status.updateState(INTERNALERR);
	}
	restaurant.addFdHandler(_pipeFDs[0], this, EPOLLIN);
	restaurant.addFdHandler(_pipeFDs[1], this, EPOLLOUT);	
}

void	Dish::_trashDish()
{
	body = "";
	close(_inFD);
	restaurant.removeFdHandler(_pipeFDs[0]);
	restaurant.removeFdHandler(_pipeFDs[1]);
	close(_pipeFDs[0]);
	close(_pipeFDs[1]);
	Log::getInstance().print("Dish " + std::to_string(_inFD) + " destroyed");
}

void	Dish::doError()
{
	_trashDish();

	std::string errfile = recipe.errorPaths[status.getState()];
	_inFD = open(errfile.c_str(), O_RDONLY);
	if (_inFD < 0 || status.getState() == COUNT)
	{
		status.updateState(INTERNALERR);
		body = "500 webserve encountered a critical internal error";
		_generateHeader();
		std::string response = header + body;
		customer.eat();
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
	header += "HTTP/1.1 " + std::to_string(status.getStatNum()) + " " + status.getStatMessage() + "\r\n";
	header += "Content-Type: text/html \r\n";
	header += "Content-Length: " + std::to_string(body.size()) + "\r\n";
	header += "Connection: Closed\r\n\r\n";
}
