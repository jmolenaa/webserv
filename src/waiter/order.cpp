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

#include <unistd.h>

#include "order.hpp"
#include "log.hpp"
#include "cook.hpp"
#include "dish.hpp"
#include "waiter.hpp"
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

Order::Order(void* wait, int fd) : waiter(wait), _orderFD(fd), _done(false), _dish(nullptr)
{}

Order::~Order()
{
	if (_dish != nullptr)
		delete _dish;
}

status Order::input(int eventFD)
{
	if (eventFD != _orderFD)
		throw WebservException("Order fired bad input FD event\n");
	if (this->_header.empty())
		_extractHeader();
	else
		_extractBody();
	if (this->_done)
	{
		Log::getInstance().print("\nGOT REQUEST:\n" + this->_header + this->_body);
		Waiter* wait = (Waiter*)waiter;
		const Cook* cook = wait->kitchen.find(this->_hostname);
		if (cook == nullptr)
			cook = wait->kitchen.begin();

		std::string page = this->_path;
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
		this->_dish = new Dish(this->_status, this, recipe);
		send(this->_orderFD, _dish->tmpGetResponse().c_str(), _dish->tmpGetResponse().size(), 0);
		close(this->_orderFD);
		wait->finishOrder(this->_orderFD);
	}
	return (OK);
}

status Order::output(int eventFD)
{
	if (eventFD != this->_orderFD)
		throw WebservException("Order fired bad output FD\n");
	// if (this->_dish == nullptr)
	// 	throw WebservException("Order fired bad output without a dish FD\n");
	// //send order from dish in chunks. Consider piping dish output directly to client?
	// send(this->_orderFD, _dish->tmpGetResponse().c_str(), _dish->tmpGetResponse().size(), 0);
	// Waiter* wait = (Waiter*)waiter;
	// wait->finishOrder(this->_orderFD);
	return (_status.getState());
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
