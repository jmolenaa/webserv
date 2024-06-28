/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _methods.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/03 13:10:36 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/19 14:00:36 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"
#include <unistd.h>
#include "order.hpp"
#include "sstream"
#include <sys/stat.h>

static bool	isDirectory(std::string const& path) {
	struct stat	fileInfo;
	if (stat(path.c_str(), &fileInfo) != -1 && S_ISDIR(fileInfo.st_mode)) {
		return true;
	}
	return false;
}

bool Dish::_isCGIRequest() const {
	size_t		qpos = this->finalPage.find('?');
	std::string	pathNoQuery = this->finalPage;

	if (qpos != std::string::npos) {
		pathNoQuery = this->finalPage.substr(0, qpos);
	}
	if (pathNoQuery.size() > 4 && pathNoQuery.substr(pathNoQuery.size() - 4) == "." + this->recipe.cgiExtension) {
		return true;
	}
	return false;
}

void Dish::_get()
{
	if (!isDirectory(this->finalPage)) {
		if (_isCGIRequest()) {
			_CGI = new CGI(*this);
			_CGI->execute();
			return ;
		}
		this->_fdOfFileToRead = open(this->finalPage.c_str(), O_RDONLY);
	}
	else {
		if (this->finalPage[this->finalPage.size() - 1] != '/') {
			this->finalPage += "/";
		}
		//do CGI stuff here to list directory contents
		if (recipe.autoindex) {
			std::cout << "WE ARE AUTOINDEXIIIIING\n";
//			CGI cgi(*this);
//			dishFD = cgi.execute();
		}
		else {
			std::string index = this->finalPage + recipe.index;
			this->_fdOfFileToRead = open(index.c_str(), O_RDONLY);
		}
	}
	if (this->_fdOfFileToRead < 0) {
		_handleOpenError(errno);
	}
	if (this->status.getState() == OK) {
		_doPipe();
	}
}

void Dish::_post()
{
	if (order.getPath().find("/cgi-bin/post.cgi") != 0
		&& order.getPath().find("/cgi-bin/upload.cgi") != 0)
		return (status.updateState(FORBIDDEN));
	else
	{	
		_CGI = new CGI(*this);
		_CGI->execute();
	}
}

void Dish::_delete()
{
    if (order.getPath().find("/cgi-bin/delete.cgi") != 0)
        return (status.updateState(FORBIDDEN));
    else
	{
		_CGI = new CGI(*this);
		_CGI->execute();
	}
}
