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
#include "log.hpp"

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

Order::Order(Status& stat, int fd) : _status(stat), _orderFD(fd), _done(false), _headerEnd(std::string::npos), _buffer(""), _method(OTHER), _table(8080), _contentLength(0) {}

//taking the order
bool Order::makeOrder()
{
	if (_done) {
		_printData();
		return (_done);
	}
	if (_headerEnd == std::string::npos)
		_extractHeader();
	else
		_extractBody();
	return (_done);
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

//returns the full request message
std::string const& Order::getOrder() const
{
	return _order;
}

std::string Order::getBody() const
{
	return _order.substr(_headerEnd);
}
