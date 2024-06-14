/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/14 22:14:27 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "waiter.hpp"
#include "webservException.hpp"
#include "log.hpp"
#include "restaurant.hpp"

//Creates server. Removed setup functions since they are never reused.
Waiter::Waiter(Kitchen kitch, void* restaurantPointer) : FdHandler(restaurantPointer), kitchen(kitch)
{
	Log::getInstance().print("Waiter is being hired");
	
	//create socket
	this->_inFD = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_inFD < 0)
		throw WebservException("Failed to create socket because: " + std::string(std::strerror(errno)) + "\n");
	//bind to address
	int	optvalTrue = 1;
	setsockopt(this->_inFD, SOL_SOCKET, SO_REUSEADDR, &optvalTrue, sizeof(optvalTrue));
	setsockopt(this->_inFD, SOL_SOCKET, SO_REUSEPORT, &optvalTrue, sizeof(optvalTrue));
	sockaddr_in	waiterAddr{};
	waiterAddr.sin_family = AF_INET;
	waiterAddr.sin_port = this->kitchen.begin()->getTable();
	waiterAddr.sin_addr.s_addr = this->kitchen.begin()->getAddress();
	if (bind(this->_inFD, reinterpret_cast<sockaddr *>(&waiterAddr), sizeof(waiterAddr)) == -1)
		throw WebservException("Failed to bind to socket because: " + std::string(std::strerror(errno)) + "\n");
	//listen on socket
	if (listen(this->_inFD, SOMAXCONN) == -1)
		throw WebservException("Waiter could not listen because: " + std::string(std::strerror(errno)) + "\n");
	//add to epoll and map
	Restaurant* restaurant = (Restaurant*)this->resP;
	restaurant->addFdHandler(this->_inFD, this, EPOLLIN);

	Log::getInstance().print("Waiter " + std::to_string(_inFD) + " is working at table " + std::to_string(ntohs(this->kitchen.begin()->getTable())) + " with " + std::to_string(this->kitchen.size()) + " cooks in the kitchen");
}

Waiter::~Waiter()
{
	for (auto order : _orders)
		delete (order.second);
}

//accept the order
void Waiter::input(int eventFD)
{
	if (eventFD != this->_inFD)
		throw WebservException("Waiter fired for bad input FD event\n");

	sockaddr_in orderAddr{};
	socklen_t 	orderAddrLen = sizeof(orderAddr);
	int orderFD = accept(_inFD, reinterpret_cast<sockaddr *>(&orderAddr), &orderAddrLen);
	if (orderFD == -1)
		throw WebservException("Failed to take a new order: " + std::string(std::strerror(errno)) + "\n");

	Order* order = new Order(this, orderFD, resP);
	this->_orders[orderFD] = order;
}

void Waiter::output(int eventFD)
{
	if (eventFD != _outFD)
		throw WebservException("Waiter fired bad output FD event: " + std::to_string(eventFD) + "\n");
}

void Waiter::finishOrder(int orderFD)
{
	Log::getInstance().print("Finishing order " + std::to_string(orderFD));
	Restaurant* restaurant = (Restaurant*)resP;
	restaurant->removeFdHander(orderFD);
	if (this->_orders.find(orderFD) != this->_orders.end())
	{
		this->_orders.erase(orderFD);
		delete (this->_orders[orderFD]);
	}
	throw WebservException("Order does not exist and could not be erased\n");
}