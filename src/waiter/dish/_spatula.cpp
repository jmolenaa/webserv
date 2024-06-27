/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _spatula.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/24 12:25:55 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/26 16:38:14 by yizhang       ########   odam.nl         */
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
	this->_inFD = _pipeFDs[0];
	this->_outFD = _pipeFDs[1];
	restaurant.addFdHandler(this->_inFD, this, EPOLLIN | EPOLLHUP | EPOLLERR);
	restaurant.addFdHandler(this->_outFD, this, EPOLLOUT);
}

void Dish::_removeHandlers() {
	if (this->_inFD != -1) {
		restaurant.removeFdHandler(this->_inFD);
		close(this->_inFD);
		this->_inFD = -1;
	}
	if (this->_outFD != -1) {
		restaurant.removeFdHandler(this->_outFD);
		close(this->_outFD);
		this->_outFD = -1;
	}
}

void	Dish::_trashDish()
{
	body = "";
	this->_removeHandlers();
	Log::getInstance().print("Dish " + std::to_string(this->_fdOfFileToRead) + " destroyed");
	if (this->_fdOfFileToRead != -1) {
		close(this->_fdOfFileToRead);
	}
//	restaurant.removeFdHandler(_pipeFDs[1]);
//	close(_pipeFDs[0]);
//	close(_pipeFDs[1]);
}

void	Dish::doError()
{
	_trashDish();

	std::string errFile = recipe.errorPaths[status.getState()];
	this->_fdOfFileToRead = open(errFile.c_str(), O_RDONLY);
	if (this->_fdOfFileToRead < 0 || status.getState() == COUNT)
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
//		input(this->_inFD);
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
