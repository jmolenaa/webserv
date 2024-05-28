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
 * @todo
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
Order::Order(char *order) : _order(order)
{
	_extractHeader();
	_extractMethod();
	_extractPath();
	_extractHost();
	_extractBody();
	_printData();
}

method& Order::getMethod()
{
	return _method;
}

std::string& Order::getPath()
{
	return _page;
}

std::string& Order::getCookName()
{
	return _hostname;
}

uint& Order::getTable()
{
	return _table;
}

uint& Order::getLength()
{
	return _contentLength;
}

std::string& Order::getBody()
{
	return _body;
}
