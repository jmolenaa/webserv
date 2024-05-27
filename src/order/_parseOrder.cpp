/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _parseOrder.cpp                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/27 16:20:46 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 16:22:16 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "order.hpp"
#include "log.hpp"

std::string Order::_keyValueFind(std::string string, std::string key, char delim)
{
	size_t start = string.find(key);
	if (start == std::string::npos)
		return ("");
	start += key.length();
	size_t end = string.find_first_of(delim, start);
	return(string.substr(start, end - start));
}

void Order::_extractHeader()
{
	uint pos = _order.find("\r\n\r\n");
	if (pos == (unsigned int)std::string::npos)
		_header = _order;
	else
		_header = _order.substr(0, pos);
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
	_hostname = _keyValueFind(_header, "Host: ", ':');
    _port = PORT;
	std::string tmp = _keyValueFind(_header, "Host: ", '\n');
	tmp = _keyValueFind(tmp, ":", '\n');
	if (!tmp.empty())
		_port = std::stoi(tmp);
}

void Order::_extractBody()
{
    _contentLength = 0;
	std::string tmp = _keyValueFind(_order, "Content-Length: ", '\n');
	if (!tmp.empty())
		_contentLength = std::stoi(tmp);
	
	if (_contentLength)
	{
		uint pos = _order.find("\r\n\r\n");
		if (pos == (unsigned int)std::string::npos)
			_body = "";
		else
			_body = _order.substr(pos + 4,_contentLength);
	}
}

void Order::_printData()
{
	if (Log::getInstance().isEnabled())
	{
		std::string data =  "=====GOT ORDER=====\n";
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
		data += "\nPath: '" + _path + "'"
			+ "\nHost: '" + _hostname + "'";
			+ "\nPort: '" + std::to_string(_port) + "'"
			+ "\nLength: '" + std::to_string(_contentLength) + "'"
			+ "\nBody: '" + _body + "'"
			+ "\n=====END OF ORDER=====\n";
		
		Log::getInstance().print(data);
	}
}
