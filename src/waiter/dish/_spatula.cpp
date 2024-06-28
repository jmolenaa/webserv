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
		status.updateState(COUNT);
		return ;
	}
	this->_inFD = _pipeFDs[0];
	this->_outFD = _pipeFDs[1];
	restaurant.addFdHandler(this->_outFD, this, EPOLLOUT);
	restaurant.addFdHandler(this->_inFD, this, EPOLLIN | EPOLLHUP | EPOLLERR);
}

void Dish::_removeHandler(int& handlerFd) {
	if (handlerFd != -1) {
		restaurant.removeFdHandler(handlerFd);
		close(handlerFd);
		handlerFd = -1;
	}
}

void	Dish::_trashDish()
{
	body = "";
	this->_removeHandler(this->_outFD);
	this->_removeHandler(this->_inFD);
	if (this->_fdOfFileToRead != -1) {
		close(this->_fdOfFileToRead);
		Log::getInstance().print("Dish " + std::to_string(this->_fdOfFileToRead) + " destroyed");
		this->_fdOfFileToRead = -1;
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
