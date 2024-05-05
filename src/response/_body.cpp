/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _body.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:10:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/03 16:49:33 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"

//get filetype from _path
void	Response::_findFileType()
{
	size_t	dir = _path.find_last_of('/');
	if (dir == std::string::npos)
	{
		_status._state = Status::BAD;
		return;
	}
	size_t pos = _path.find_last_of('.', dir);
	if (pos != std::string::npos)
	{
		_filetype = _path.substr(pos + 1);
		if (_filetype != "html" && _filetype != "php")
			_status._state = Status::UNSUPPORTED;
		return;
	}
	// if show location is on, change filetype to folder?
}

//populates response body
void Response::_populateBody()
{
	_findFileType();
	_body += "\r\n";
	if (_filetype == "PHP")
	{
		//Do CGI stuff
		_body += "CGI STUFF NOT YET SUPPORTED";
		return;
	}
	if (_filetype == "html")
	{
		//change this to use locations
		std::string root = "/home/daoyi/codam/webserv/pages";
		root += _path;
		std::ifstream file(root);
		std::string line;
		if (file.is_open())
		{
			std::cout << "FOUND IT!!!!!!!!!!!!!!!!" << std::endl;
			while (std::getline(file, line))
				_body += line + "\n";
			file.close();
		}
	}
	if (_filetype == "none")
	{
		//update to use locations
		std::string root = "/home/daoyi/codam/webserv/pages";
		std::ifstream file(root.append("index.html"));
		std::string line;
		if (file.is_open())
		{
			while (std::getline(file, line))
				_body += line + "\n";
			file.close();
		}
		else
		{
			//if show folder is on, show the folder, else 404
			_status._state = Status::NOTFOUND;
		}
	}
	if (_status._state != Status::OK)
		return (_populateError());
}
