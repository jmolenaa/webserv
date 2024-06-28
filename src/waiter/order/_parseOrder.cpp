/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _parseOrder.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/27 16:20:46 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/26 12:55:43 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "order.hpp"

#include <unistd.h>
#include "log.hpp"
#include "defines.hpp"

void Order::_extractHeader()
{
	ssize_t count = read(_orderFD, _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		_done = true;
		_order += "\r\n\r\n";
		_status.updateState(INTERNALERR);
	}
	else if (count == 0) { //TODO change this
		sleep(1);
	}
	else
	{
		_bufStr.append(_buffer, count);
		_headerEnd = _bufStr.find("\r\n\r\n");
		if (_headerEnd == std::string::npos) {
			return;
		}
		_parseHeader();
		if (_status.getState() != OK) {
			_done = true;
		}
	}
}

void Order::_parseHeader()
{
	_headerEnd += 4;
	_order = _bufStr.substr(0, _headerEnd);
	
	_extractMethod();
	_extractPath();
	_extractHost();

    _contentLength = 0;
	std::string tmp = _extractValue(_order, "Content-Length: ", '\n');
	if (!tmp.empty())
		_contentLength = std::stoi(tmp);
 //TODO ask about this if statement
	if (_contentLength > MAX_BODY_SIZE)
		return (_status.updateState(UNSUPPORTED));
	if (_contentLength)
		_contentType = _extractValue(_order, "Content-Type: ", '\n');

	if (_headerEnd != _bufStr.size()) {
		_order += _bufStr.substr(_headerEnd);
	}
	_bufStr.clear();

	if (_contentLength == (_order.size() - _headerEnd))	{
		_printData();
		_done = true;
	}
}

// TODO, what if whatever is requested contains GET, POST, DELETE
void Order::_extractMethod()
{
	size_t	method_pos;
	
	method_pos = _order.find("GET");
	if (method_pos != std::string::npos)
	{
		_method = GET;
		return;
	}
	method_pos = _order.find("POST");
	if (method_pos != std::string::npos)
	{
		_method = POST;
		return;
	}
	method_pos = _order.find("DELETE");
	if (method_pos != std::string::npos)
	{
		_method = DELETE;
		return;
	}
	_method = OTHER;
}

void Order::_extractPath()
{
	size_t	path_start = _order.find_first_of('/');
	if (path_start != std::string::npos)
	{
		size_t path_end = _order.find_first_of(' ', path_start);
		_path = _order.substr(path_start, path_end - path_start);
		return;
	}
	_path = "";
}

void Order::_extractHost()
{
	_hostname = _extractValue(_order, "Host: ", ':');
    _table = PORT;
	std::string tmp = _extractValue(_order, "Host: ", '\n');
	tmp = _extractValue(tmp, ":", '\n');
	if (!tmp.empty())
		_table = std::stoi(tmp);
}

void Order::_extractBody()
{
	Log::getInstance().print("Getting body---------");
	ssize_t count = read(_orderFD, _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		_done = true;
		_status.updateState(INTERNALERR);		
	}
	else if (count == 0) { //TODO change this
		sleep(1);
	}
	else
	{
		_order.append(_buffer, count);
		if (_order.size() - _headerEnd >= _contentLength)
		{
			_done = true;
			_printData();
		}
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
			+ "\n	Body: '" + _order.substr(_headerEnd) + "'"
			+ "\n=====END OF ORDER=====\n";
		
		Log::getInstance().print(data);
//		Log::getInstance().print("Full header:\n" + this->);
//		Log::getInstance().print("Full body:\n" + this->_order);
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
