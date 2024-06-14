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

#include "order.hpp"
#include <unistd.h>

#include "log.hpp"
#include "restaurant.hpp"
#include "cook.hpp"
#include "waiter.hpp"
#include "dish.hpp"
#include "webservException.hpp"

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

Order::Order(void* waiterPointer, int fd, void* restaurantPointer) 
	: FdHandler(restaurantPointer), _wP(waiterPointer), _done(false)
{
	this->_inFD = fd;

	Waiter* waiter = (Waiter*)_wP;
	Log::getInstance().print("Waiter " + std::to_string(waiter->getIn()) + " is taking order " + std::to_string(fd) + "\n");
	
	Restaurant* restaurant = (Restaurant*)this->resP;
	restaurant->addFdHandler(_inFD, this, EPOLLIN);
}

Order::~Order() {}

//taking the order
void Order::input(int eventFD)
{
	if (eventFD != _inFD)
		throw WebservException("Order fired bad input FD event\n");
	if (this->_header.empty())
		_extractHeader();
	else
		_extractBody();
	if (this->_done)
	{
		Restaurant* restaurant = (Restaurant*)this->resP;
		restaurant->removeFdHander(_inFD);
		Log::getInstance().print("\nGOT REQUEST:\n" + this->_header + this->_body);
		Waiter* waiter = (Waiter*)_wP;
		waiter->prepOrder(_outFD);
	}
}

void Order::output(int eventFD)
{
	if (eventFD != this->_outFD)
		throw WebservException("Order fired bad output FD\n");
	// if (this->_dish == nullptr)
	// 	throw WebservException("Order fired bad output without a dish FD\n");
	// //send order from dish in chunks. Consider piping dish output directly to client?
	// send(this->_orderFD, _dish->tmpGetResponse().c_str(), _dish->tmpGetResponse().size(), 0);
	// Waiter* wait = (Waiter*)waiter;
	// wait->finishOrder(this->_orderFD);
	// return (_status.getState());
}

void Order::orderDone()
{
	Waiter* waiter = (Waiter*)_wP;
	close(_inFD);
	waiter->finishOrder(_inFD);
}

//Returns the method GET, POST, DELETE
method Order::getMethod() const
{
	return _method;
}

//Returns the request path
std::string Order::getPath() const
{
	return _path;
}

//returns the Port number
uint Order::getTable() const
{
	return _table;
}

//returns the hostname
std::string Order::getHostname() const
{
	return _hostname;
}

//returns CONTENT_LENGTH
uint Order::getLength() const
{
	return _contentLength;
}

//returns CONTENT_TYPE
std::string Order::getType() const
{
	return _contentType;
}

//returns the body of the request only
std::string Order::getBody() const
{
	return _body;
}

//returns the full request message
std::string Order::getOrder() const
{
	return _header + _body;
}