/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _methods.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:10:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/18 17:09:09 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"
#include <unistd.h>
#include "order.hpp"
#include "sstream"
#include <sys/stat.h>

/**
 * @todo autoindex stuff
 * 
 */

//static bool	isDirectory(std::string const& path) {
//	struct stat	fileInfo;
//	stat(path.c_str(), &fileInfo);
//	if (S_ISDIR(fileInfo.st_mode)) {
//		return true;
//	}
//	return false;
//}

void Dish::_get()
{

	std::string page = _recipe.root + _order.getPath();
	size_t pos = page.find_last_of('.');
	if (pos != std::string::npos)
	{
		std::string type = page.substr(pos + 1);
		if (type != "html")
			return (_status.updateState(UNSUPPORTED));

		_inFD = open(page.c_str(), O_RDONLY);
		if (_inFD < 0)
			_doError();
	}
	else
	{
		//if !isDirectory()
		//return (_status.updateState(UNSUPPORTED));

		// if (_recipe.autoindex)
		//do CGI stuff here to list directory contents
		// else
		// {
		std::string index = page + _recipe.index;
		_inFD = open(index.c_str(), O_RDONLY);
		// }
	}
	if (_inFD < 0)
		return (_status.updateState(NOTFOUND));

	_doPipe();
	input(_inFD);
//	std::string page = _recipe.root + _order.getPath();
//	if (!isDirectory(page)) {
//		std::string type = page.substr(pos + 1);
//		if (type != "html")
//			return (_status.updateState(UNSUPPORTED));
//
//		_inFD = open(page.c_str(), O_RDONLY);
//		if (_inFD < 0)
//			_doError();
//	} else {
//		if (page[page.size() - 1] != '/') {
//			page += "/";
//		}
//		std::cout << "---------------------------WE DOING THIS>>>>>?\n";
//		//do CGI stuff here to list directory contents
//		if (_recipe.autoindex) {
//			std::cout << "WE ARE AUTOINDEXIIIIING\n";
//			CGI cgi(*this);
//			_dishFD = cgi.execute();
//		} else {
//			std::cout << "not\n";
//			std::string index = page + _recipe.index;
//		}
//	}
}

void Dish::_post()
{
	if (_order.getPath().find("/cgi-bin/post.cgi") != 0
		&& _order.getPath().find("/cgi-bin/upload.cgi") != 0)
		return (_status.updateState(FORBIDDEN));
	else
	{	
		CGI cgi(*this);
		_inFD = cgi.execute();
	}
}

void Dish::_delete()
{
    if (_order.getPath().find("/cgi-bin/delete.cgi") != 0)
        return (_status.updateState(FORBIDDEN));
    else
	{
		CGI cgi(*this);
		_inFD = cgi.execute();
	}
}
