/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _parseOrder.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/27 16:20:46 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/14 17:44:17 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

#include "order.hpp"
#include "waiter.hpp"
#include "log.hpp"

void Order::_extractHeader()
{
	ssize_t count = read(_inFD, _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		_done = true;
		_header += "\r\nERR";
		status.updateState(INTERNALERR);
	}
	else
	{
		_buffer[count] = '\0';
		_bufStr += std::string(_buffer);
		size_t pos = _bufStr.find("\r\n\r\n");
		if (pos == std::string::npos) {
			return;
		}
		pos += 4;
		_header = _bufStr.substr(0, pos);
		if (pos != _bufStr.size()) {
			_body += _bufStr.substr(pos);
		}
		_bufStr = "";
		_parseHeader();
		if (_contentLength == 0)
			_done = true;
	}
}

void Order::_parseHeader()
{
	_extractMethod();
	_extractPath();
	_extractHost();

    _contentLength = 0;
	std::string tmp = _extractValue(_header, "Content-Length: ", '\n');
	if (!tmp.empty())
		_contentLength = std::stoi(tmp);
	if (_contentLength)
		_contentType = _extractValue(_header, "Content-Type: ", '\n');
}

void Order::_extractMethod()
{
	size_t	method_pos;
	
	method_pos = _header.find("GET");
	if (method_pos != std::string::npos)
	{
		_method = GET;
		return;
	}
	method_pos = _header.find("POST");
	if (method_pos != std::string::npos)
	{
		_method = POST;
		return;
	}
	method_pos = _header.find("DELETE");
	if (method_pos != std::string::npos)
	{
		_method = DELETE;
		return;
	}
	_method = OTHER;
}

void Order::_extractPath()
{
	size_t	path_start = _header.find_first_of('/');
	if (path_start != std::string::npos)
	{
		size_t path_end = _header.find_first_of(' ', path_start);
		_path = _header.substr(path_start, path_end - path_start);
		return;
	}
	_path = "";
}

void Order::_extractHost()
{
	_hostname = _extractValue(_header, "Host: ", ':');
    _table = PORT;
	std::string tmp = _extractValue(_header, "Host: ", '\n');
	tmp = _extractValue(tmp, ":", '\n');
	if (!tmp.empty())
		_table = std::stoi(tmp);
}

void Order::_extractBody()
{
	if (_body.size() >= _contentLength)
	{
		_done = true;
		return ;
	}
	
	ssize_t count = read(_inFD, _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		_done = true;
		status.updateState(INTERNALERR);		
	}
	else
	{
		_buffer[count] = '\0';
		_body += std::string(_buffer);
	}
}

void Order::_printData()
{
	if (Log::getInstance().isEnabled())
	{
		std::string data =  "=====PLACING ORDER=====\n";
		data += "\nMethod: ";
		switch (_method)
		{
			case GET:
				data += "'GET'";
				break;
			case POST:
				data += "'POST'";
				break;
			case DELETE:
				data += "'DELETE'";
				break;
			default:
				data += "'NONE'";
		}
		data += "\n	Path: '" + _path + "'"
			+ "\n	Host: '" + _hostname + "'"
			+ "\n	Table: '" + std::to_string(_table) + "'"
			+ "\n	Length: '" + std::to_string(_contentLength) + "'"
			+ "\n	ContentType: '" + _contentType + "'"
			+ "\n	Body: '" + _body + "'"
			+ "\n=====END OF ORDER=====\n";
		
		Log::getInstance().print(data);
	}
}

std::string Order::_extractValue(std::string string, std::string key, char delim)
{
	size_t start = string.find(key);
	if (start == std::string::npos)
		return ("");
	start += key.length();
	size_t end = string.find_first_of(delim, start);
	return(string.substr(start, end - start));
}
