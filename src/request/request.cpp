/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:11:53 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/24 14:11:17 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

/** 
 * @todo
 * read from the socket until you encounter the 0x0D 0x0A sequence of bytes denoting the end of the response line, which contains the HTTP version, Status Code, and Reason text.
 * then read from the socket until you encounter the 0x0D 0x0A 0x0D 0x0A sequence of bytes denoting the end of the response headers.
 * then analyze the response line and headers to know if a message body is present, and if so in what format it is being sent as, which dictates how you must read it.
 * read the message body, if present:
 * if the response Status code is 1xx, 204, or 304, or if the response is to a HEAD request, no message body is present.
 * otherwise, if a Transfer-Encoding header is present and has a value other than identity, read the message body in chunks until a 0-length chunk is read.
 * otherwise, if a Content-Length header is present, read from the socket until the exact number of bytes specified have been read, no more, no less.
 * otherwise, if the Content-Type header indicates a multipart/... media type, read from the socket and parse the MIME data until the final terminating MIME boundary is reached.
 * otherwise, read from the socket until the connection is closed.
 * if the response is not read in full successfully, or if a keep-alive is NOT being used (a Connection: close header is present in an HTTP 1.1 response, or a Connection: keep-alive header is not present in an HTTP 1.0 response), then close the socket.
 * This is covered by RFC 2616 (Section 4.4 and Section 8), and by RFC 7230 (Section 3.3.3 and Section 6), etc
*/
Request::Request(char *request) : _request(request)
{
	_extractMethod();
	_extractHost();
	_extractBody();
}

void Request::_extractMethod()
{
	size_t	method_pos;
	std::string methods[] = {"GET", "POST", "DELETE"};
	
	for (std::string type : methods)
	{
		method_pos = _request.find(type);
		if (method_pos != std::string::npos)
		{
			_method = _request.substr(0, type.length());
			// std::cout << "\nGOT METHOD:_" << _method << "_\n";
			return;
		}
	}
	std::cerr << "Bad message from client: " << _request << std::endl;
	exit(EXIT_FAILURE);
}

void Request::_extractHost()
{
	_hostname = Helpers::_keyValueFind(_request, "Host: ", ':');
	// std::cout << "GOT HOSTNAME:_" << _hostname << "_\n";

	std::string tmp = Helpers::_keyValueFind(_request, "Host: ", '\n');
	tmp = Helpers::_keyValueFind(tmp, ":", '\n');
	if (!tmp.empty())
		_port = std::stoi(tmp);
	else
		_port = PORT;
	// std::cout << "GOT PORT:_" << _port << "_\n";
}

void Request::_extractBody()
{
	std::string tmp = Helpers::_keyValueFind(_request, "Content-Length: ", '\n');
	if (!tmp.empty())
		_contentLength = std::stoi(tmp);
	else
		_contentLength = 0;
	// std::cout << "GOT LENGTH:_" << _contentLength << "_\n";

	_body = Helpers::_keyValueFind(_request, "\r\n\r\n", 0);
	// std::cout << "GOT BODY:_" << _body << "_\n" << std::endl;
}

std::string Request::getMethod()
{
	return _method;
}

std::string Request::getHostname()
{
	return _hostname;
}

int Request::getPort()
{
	return _port;
}

int Request::getLength()
{
	return _contentLength;
}

std::string Request::getBody()
{
	return _body;
}

void Request::printData()
{
	std::cout
		<< "\nMethod: '" << _method << "'"
		<< "\nHost: '" << _hostname << "'"
		<< "\nPort: '" << _port << "'"
		<< "\nLength: '" << _contentLength << "'"
		<< "\nBody: '" << _body << "'"
	<< std::endl;
}