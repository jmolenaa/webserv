/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   waiter.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/26 16:57:30 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "waiter.hpp"

#include <cstring>
#include "webservException.hpp"
#include "log.hpp"
#include "restaurant.hpp"
#include "cook.hpp"
#include "customer.hpp"

//Creates server. Removed setup functions since they are never reused.
Waiter::Waiter(Kitchen kitch, Restaurant& rest) : FdHandler(rest, WAITERTYPE), kitchen(kitch)
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
	// Set server socket to non-blocking
	fcntl(this->_inFD, F_SETFL, O_NONBLOCK);
	if (bind(this->_inFD, reinterpret_cast<sockaddr *>(&waiterAddr), sizeof(waiterAddr)) == -1)
		throw WebservException("Failed to bind to socket because: " + std::string(std::strerror(errno)) + "\n");
	//listen on socket
	if (listen(this->_inFD, SOMAXCONN) == -1)
		throw WebservException("Waiter could not listen because: " + std::string(std::strerror(errno)) + "\n");
	//add to epoll and map
	restaurant.addFdHandler(this->_inFD, this, EPOLLIN | EPOLLHUP | EPOLLERR );

	Log::getInstance().print("Waiter " + std::to_string(_inFD) + " is working at table " + std::to_string(ntohs(this->kitchen.begin()->getTable())) + " with " + std::to_string(this->kitchen.size()) + " cooks in the kitchen\n");
}

//kick out any remaining customers
Waiter::~Waiter()
{
	for (auto customer : _customers)
		delete (customer.second);
}

//Seat the new customer
void Waiter::input(int eventFD)
{
	if (eventFD != this->_inFD)
		throw WebservException("Waiter + " + std::to_string(this->_inFD) + " input freakout.\n");

	sockaddr_in orderAddr{};
	socklen_t 	orderAddrLen = sizeof(orderAddr);
	int customerFD = accept(_inFD, reinterpret_cast<sockaddr *>(&orderAddr), &orderAddrLen);
	if (customerFD == -1) {
		if (errno == EMFILE || errno == ENFILE) {
			Log::getInstance().printErr("Server is currently busy handling other request and can't accept new clients");
			return ;
		}
		throw WebservException("Failed to seat the Customer: " + std::string(std::strerror(errno)) + "\n");
	}

	Customer* customer = new Customer(customerFD, restaurant, *this);
	this->_customers[customerFD] = customer;
}

//Is pure virtual function so have to override, we don't use it though
void Waiter::output(int customerFD)
{
	(void)customerFD;
}

void Waiter::kickCustomer(int customerFd) {
	Log::getInstance().print("Saying goodbye to customer " + std::to_string(customerFd));
	if (this->_customers.find(customerFd) == this->_customers.end()) {
		throw WebservException("Customer " + std::to_string(customerFd) + " does not exist.\n");
	}
	delete (this->_customers[customerFd]);
	this->_customers.erase(customerFd);
}
