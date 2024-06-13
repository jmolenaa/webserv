/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/13 20:14:16 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <cstring>

#include "waiter.hpp"
#include "webservException.hpp"
#include "log.hpp"
#include "restaurant.hpp"

Waiter::Waiter(Kitchen kitch, void* rest) : kitchen(kitch), restaurant(rest)
{
	Log::getInstance().print("Waiter is being hired");
	//create socket
	this->_waiterFD = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_waiterFD < 0)
		throw WebservException("Failed to create socket because: " + std::string(std::strerror(errno)) + "\n");
	//bind to address
	int	optvalTrue = 1;
	setsockopt(this->_waiterFD, SOL_SOCKET, SO_REUSEADDR, &optvalTrue, sizeof(optvalTrue));
	setsockopt(this->_waiterFD, SOL_SOCKET, SO_REUSEPORT, &optvalTrue, sizeof(optvalTrue));
	sockaddr_in	waiterAddr{};
	waiterAddr.sin_family = AF_INET;
	Log::getInstance().print("Waiter is waiting at table " + std::to_string(ntohs(this->kitchen.begin()->getTable())));
	waiterAddr.sin_port = this->kitchen.begin()->getTable();
	waiterAddr.sin_addr.s_addr = this->kitchen.begin()->getAddress();
	if (bind(this->_waiterFD, reinterpret_cast<sockaddr *>(&waiterAddr), sizeof(waiterAddr)) == -1)
		throw WebservException("Failed to bind to socket because: " + std::string(std::strerror(errno)) + "\n");
	//listen on socket
	if (listen(this->_waiterFD, SOMAXCONN) == -1)
		throw WebservException("Waiter could not listen because: " + std::string(std::strerror(errno)) + "\n");
	//add to epoll and map
	Restaurant* restu = (Restaurant*)this->restaurant;
	restu->addFdHandler(this->_waiterFD, this, EPOLLIN);

	Log::getInstance().print("Waiter is working with " + std::to_string(this->kitchen.size()) + " Cooks in the kitchen");
}

Waiter::~Waiter()
{
	for (auto order : _orders)
		delete (order.second);
		
    if (this->_waiterFD > 0)
        close(this->_waiterFD);
}

status Waiter::input(int eventFD)
{
	if (eventFD == this->_waiterFD)
	{
		sockaddr_in customerAddr{};
		socklen_t 	customerAddrLen = sizeof(customerAddr);
		int customerFD = accept(_waiterFD, reinterpret_cast<sockaddr *>(&customerAddr), &customerAddrLen);
		if (customerFD == -1)
			throw WebservException("Failed to welcome a new customer: " + std::string(std::strerror(errno)) + "\n");
		
		Order* order = new Order(this, customerFD);
		this->_orders[customerFD] = order;

		Restaurant* restu = (Restaurant*)this->restaurant;
		restu->addFdHandler(customerFD, this->_orders[customerFD], EPOLLIN | EPOLLOUT);
		return (OK);
	}
	else
		throw WebservException("Waiter fired bad input FD event\n");
	return (INTERNALERR);
}

status Waiter::output(int eventFD)
{
	throw WebservException("Waiter fired bad output FD event: " + std::to_string(eventFD) + "\n");
}

void Waiter::finishOrder(int orderFD)
{
	if (this->_orders.find(orderFD) != this->_orders.end())
	{
		delete (this->_orders[orderFD]);
		this->_orders.erase(orderFD);
	}
	throw WebservException("Order does not exist and could not be erased\n");
}