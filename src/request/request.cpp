/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/23 17:11:53 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/02 11:45:22 by dliu          ########   odam.nl         */
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
	//something neds to update the return code for the header, possible throw
	_extractMethod();
	_extractPath();
	_extractHost();
	_extractBody();
}

void Request::_extractMethod()
{
	size_t	method_pos;
	std::string methods[] = {"GET", "POST", "DELETE"};
	
	for (std::string& type : methods)
	{
		method_pos = _request.find(type);
		if (method_pos != std::string::npos)
		{
			_method = _request.substr(0, type.length());
			return;
		}
	}
	_method = "";
	std::cerr << "Cannot parse request from client." << std::endl;
}

void Request::_extractPath()
{
	size_t	path_start = _request.find_first_of('/');
	if (path_start != std::string::npos)
	{
		size_t path_end = _request.find_first_of(' ', path_start);
		_path = _request.substr(path_start, path_end - path_start);
		return;
	}
	_path = "";
	std::cerr << "Cannot parse request from client" << std::endl;
}

void Request::_extractHost()
{
	_hostname = Helpers::_keyValueFind(_request, "Host: ", ':');

    _port = PORT;
	std::string tmp = Helpers::_keyValueFind(_request, "Host: ", '\n');
	tmp = Helpers::_keyValueFind(tmp, ":", '\n');
	if (!tmp.empty())
		_port = std::stoi(tmp);
}

void Request::_extractBody()
{
    _contentLength = 0;
	std::string tmp = Helpers::_keyValueFind(_request, "Content-Length: ", '\n');
	if (!tmp.empty())
		_contentLength = std::stoi(tmp);

	_body = Helpers::_keyValueFind(_request, "\r\n\r\n", 0);
}

std::string& Request::getMethod()
{
	return _method;
}

std::string& Request::getPath()
{
	return _path;
}

std::string& Request::getHostname()
{
	return _hostname;
}

uint& Request::getPort()
{
	return _port;
}

uint& Request::getLength()
{
	return _contentLength;
}

std::string& Request::getBody()
{
	return _body;
}

void Request::printData()
{
	std::cout
		<< "\nMethod: '" << _method << "'"
		<< "\nPath: '" << _path << "'"
		<< "\nHost: '" << _hostname << "'"
		<< "\nPort: '" << _port << "'"
		<< "\nLength: '" << _contentLength << "'"
		<< "\nBody: '" << _body << "'"
	<< std::endl;
}