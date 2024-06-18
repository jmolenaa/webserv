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

#include "log.hpp"
#include "webservException.hpp"
#include "restaurant.hpp"
#include "cook.hpp"
#include "waiter.hpp"

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

Customer::Customer(int fd, void* restaurantPointer, void* waiterPointer) : FdHandler(restaurantPointer), _wP(waiterPointer), _order(_status, fd)
{
	this->_inFD = fd;
	this->_outFD = fd;

	_pos = 0;
	_bitesLeft = 0;
	Restaurant* restaurant = (Restaurant*)this->resP;
	restaurant->addFdHandler(_inFD, this, EPOLLIN);
}

Customer::~Customer()
{
	delete 	_dish;
}

//taking the order
void Customer::input(int eventFD)
{
	if (eventFD != _inFD) {
		throw WebservException("Bad input FD event in customer\n");
	}
	Log::getInstance().print("Customer receiving input from fd " + std::to_string(eventFD));
	bool doneMakingOrder = _order.makeOrder();
	if (!doneMakingOrder) {
		Log::getInstance().print("Not receiveved order yet\n");
		return;
	}
	_getDish();
}

void Customer::_getDish()
{
	Restaurant* restaurant = (Restaurant*)this->resP;
	restaurant->removeFdHander(_inFD);

	Waiter* waiter = (Waiter*)_wP;
	const Cook* cook = waiter->kitchen.find(_order.getHostname());
	if (cook == nullptr)
		cook = waiter->kitchen.begin();

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
	_dish = new Dish(this->_status, this->_order, recipe, this->resP, this);
	_dish->doMethod();
}

void Customer::eat()
{
	_food = _dish->getDish();
	_bitesLeft = _food.size();
	delete _dish;

	//prepare to send to client
	Log::getInstance().print("Eating dish " + std::to_string(_inFD));
	Restaurant* restaurant = (Restaurant*)resP;
	restaurant->addFdHandler(_outFD, this, EPOLLOUT);
	output(_outFD);
}


void Customer::output(int eventFD)
{
	if (eventFD != this->_outFD)
		throw WebservException("Bad output FD event on Customer\n");
	
	if (_bitesLeft <= 0) {
		return (_leave());
	}

	const char* response = _food.c_str();
	response += _pos;
	size_t	size = std::strlen(response);
	if (size > BUF_LIMIT)
		size = BUF_LIMIT;
	_pos += size;
	ssize_t sent = send(_outFD, response, size, 0);
	if (sent < 0) {
		return (_leave());
	}
	_bitesLeft -= sent;
}

void Customer::_leave()
{
	Restaurant* restaurant = (Restaurant*)this->resP;
	restaurant->removeFdHander(_outFD);
	close(_outFD);

	Waiter* waiter = (Waiter*)_wP;
	waiter->output(_outFD);
}

//find the correct recipe / location
// void Customer::_prepDish()
// {
// 	Waiter* waiter = (Waiter*)_wP;
// 	const Cook* cook = waiter->kitchen.find(_hostname);
// 	if (cook == nullptr)
// 		cook = waiter->kitchen.begin();
// 	std::string page = _path;
// 	Recipe recipe(cook->getRecipe(page));
// 	while (!page.empty() && page != recipe.page) //double check this shit
// 	{
// 		size_t end = page.find_last_of('/');
// 		if (end == std::string::npos)
// 			break;
// 		else
// 			page = page.substr(0, end);
// 		recipe = cook->getRecipe(page);
// 	}
// 	_dish = new Dish(*this, recipe, this->resP);
// }

// void Customer::orderDone()
// {
// 	Waiter* waiter = (Waiter*)_wP;
// 	close(_inFD);
// 	waiter->finishCustomer(_inFD);
// }

// //Returns the method GET, POST, DELETE
// method Customer::getMethod() const
// {
// 	return _method;
// }

// //Returns the request path
// std::string Customer::getPath() const
// {
// 	return _path;
// }

// //returns the Port number
// uint Customer::getTable() const
// {
// 	return _table;
// }

// //returns the hostname
// std::string Customer::getHostname() const
// {
// 	return _hostname;
// }

// //returns CONTENT_LENGTH
// uint Customer::getLength() const
// {
// 	return _contentLength;
// }

// //returns CONTENT_TYPE
// std::string Customer::getType() const
// {
// 	return _contentType;
// }

// //returns the body of the request only
// std::string Customer::getBody() const
// {
// 	return _body;
// }

// //returns the full request message
// std::string Customer::getCustomer() const
// {
// 	return _header + _body;
// }
