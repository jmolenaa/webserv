/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   order.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:11:53 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 15:30:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "customer.hpp"
#include <unistd.h>
#include <cstring>

#include "webservException.hpp"
#include "concierge.hpp"
#include "restaurant.hpp"
#include "log.hpp"
#include "cook.hpp"

/** 
 * read from the socket until you encounter the 0x0D 0x0A sequence of bytes denoting the end of the dish line, which contains the HTTP version, Status Code, and Reason text.
 * then read from the socket until you encounter the 0x0D 0x0A 0x0D 0x0A sequence of bytes denoting the end of the dish headers.
 * then analyze the dish line and headers to know if a message body is present, and if so in what format it is being sent as, which dictates how you must read it.
 * read the message body, if present:
 * if the dish Status code is 1xx, 204, or 304, or if the dish is to a HEAD order, no message body is present.
 * otherwise, if a Transfer-Encoding header is present and has a value other than identity, read the message body in chunks until a 0-length chunk is read.
 * otherwise, if a Content-Length header is present, read from the socket until the exact number of bytes specified have been read, no more, no less.
 * otherwise, if the Content-Type header indicates a multipart/... media type, read from the socket and parse the MIME data until the final terminating MIME boundary is reached.
 * otherwise, read from the socket until the connection is closed.
 * if the dish is not read in full successfully, or if a keep-alive is NOT being used (a Connection: close header is present in an HTTP 1.1 dish, or a Connection: keep-alive header is not present in an HTTP 1.0 dish), then close the socket.
 * This is covered by RFC 2616 (Section 4.4 and Section 8), and by RFC 7230 (Section 3.3.3 and Section 6), etc
*/

Customer::Customer(int fd, Restaurant& rest, Waiter& wait) : FdHandler(rest), _waiter(wait), _order(_status, fd), _dish(nullptr),
															_bitesLeft(0), _pos(0), _customerFd(fd)
{
	this->_inFD = _customerFd;
//	this->_outFD = fd;

	_startTime = std::chrono::high_resolution_clock::now();
	restaurant.addFdHandler(_inFD, this, EPOLLIN | EPOLLHUP | EPOLLERR ); //Yixin added EPOLLHUP here
	Log::getInstance().print("Customer " + std::to_string(_inFD) + " has been seated.");
}

Customer::~Customer()
{
	delete 	_dish;
	if (this->_inFD != -1) {
		this->restaurant.removeFdHandler(this->_inFD);
		this->_inFD = -1;
	}
	if (this->_outFD != -1) {
		this->restaurant.removeFdHandler(this->_outFD);
		this->_outFD = -1;
	}
	close(_customerFd);
//	close(_inFD);
//	close(_outFD);
	Log::getInstance().print("Customer " + std::to_string(this->_customerFd) + " has left.\n----------------------------\n");
}

//taking the order
void Customer::input(int eventFD)
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
	std::chrono::time_point<std::chrono::high_resolution_clock> stop;
	start = std::chrono::high_resolution_clock::now();
	if (eventFD != _inFD) {
		throw WebservException("Bad input FD event in customer\n");
	}
	Log::getInstance().print("Customer " + std::to_string(eventFD) + " is placing an order");
	bool doneMakingOrder = _order.makeOrder();
	if (!doneMakingOrder) {
		stop = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
		std::cout<<duration.count()<<std::endl;
		if (duration.count() >= 5)
		{
			if (eventFD)
			{
				restaurant.removeFdHandler(eventFD);
				close(eventFD);
			}
			return ;
		}	
		else		
			Log::getInstance().print("Customer " + std::to_string(eventFD) + " busy placing an order\n");
		return;
	}
	restaurant.removeFdHandler(_inFD);
	_inFD = -1;
	_getDish();
}

void Customer::_getDish()
{
	const Cook* cook = _waiter.kitchen.find(_order.getHostname());
	if (cook == nullptr)
		cook = _waiter.kitchen.begin();

	std::string page = _order.getPath();
	Recipe recipe(cook->getRecipe(page));
	while (!page.empty() && page != recipe.page) //double check this shit
	{
		size_t end = page.find_last_of('/');
		if (end == std::string::npos)
			break;
		else
			page = page.substr(0, end);
		recipe = cook->getRecipe(page);
	}

	_dish = new Dish(this->_status, this->_order, recipe, *this);
	_dish->doMethod();
}

void Customer::eat()
{
	_food = _dish->getDish();
	_bitesLeft = _food.size();
	std::cout << _bitesLeft;
	std::cout <<"\nTHIS IS HERERE)@)(*$*(#&(%&)(#&%)(&#)%&'" << _food << "\n";

	//prepare to send to client
	this->_outFD = this->_customerFd;
	Log::getInstance().print("Serving to customer " + std::to_string(_outFD));
	restaurant.addFdHandler(_outFD, this, EPOLLOUT);
}

void Customer::output(int eventFD)
{
	if (eventFD != this->_outFD)
		throw WebservException("Bad output FD event on Customer\n");


	const char* response = _food.c_str();
	response += _pos;
	size_t	size = _food.size() - _pos;
	if (size > BUF_LIMIT)
		size = BUF_LIMIT;
	_pos += size;
	std::cout << size << "\n";
	ssize_t sent = send(_outFD, response, size, 0);
	Log::getInstance().print("Customer " + std::to_string(_outFD) + " is eating " + std::to_string(size) + " ingredients\n");
	if (sent < 0) {
		return (_leave());
	}
	_bitesLeft -= sent;
		
	if (_bitesLeft <= 0) {
		return (_leave());
	}

}

void Customer::_leave()
{
//	restaurant.removeFdHandler(_outFD);
//	this->_outFD = -1;
	_waiter.kickCustomer(this->_customerFd);
//	close(this->_customerFd);
}
