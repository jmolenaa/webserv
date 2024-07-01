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
#include "customer.hpp"
#include "order.hpp"
#include <sys/stat.h>
#include <cstdio>

static bool	isDirectory(std::string const& path) {
	struct stat	fileInfo;
	if (stat(path.c_str(), &fileInfo) != -1 && S_ISDIR(fileInfo.st_mode)) {
		return true;
	}
	return false;
}

bool Dish::isRightCGIExtenion(const std::string &executable) const {
	if (executable.size() > 4 && executable.substr(executable.size() - 4) == "." + this->recipe.cgiExtension) {
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
	if (isRightCGIExtenion(pathNoQuery)) {
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
		if (recipe.autoindex) {
			_autoindex();
			return ;
		}
		else {
			std::string index = this->finalPage + recipe.index;
			this->_fdOfFileToRead = open(index.c_str(), O_RDONLY);
		}
	}
	if (this->_fdOfFileToRead < 0) {
		_handleFileError(errno);
	}
	if (this->status.getState() == OK) {
		_doPipe();
	}
}

void Dish::_post()
{
	if (this->order.getType().find("multipart/form-data;") != std::string::npos) {
		std::cout << BLUE << this->recipe.uploadCgi << "\n";
		_CGI = new CGI(*this, this->recipe.uploadCgi);
	}
	else {
		_CGI = new CGI(*this);
	}
	_CGI->execute();
}

void Dish::_delete()
{
	if (std::remove(this->finalPage.c_str()) == 0) {
		this->body = "File deleted succesfully\n";
		this->customer.eat();
		return ;
	}
	else if (access(this->finalPage.c_str(), F_OK) == -1) {
		this->status.updateState(NOTFOUND);
	}
	else if (errno == EACCES) {
		this->status.updateState(FORBIDDEN);
	}
	else {
		this->status.updateState(INTERNALERR);
	}
}

void Dish::_redirect() {
	this->status.updateState(this->recipe.redir.first);
	header += "Location: " + this->recipe.redir.second + "\r\n";
	this->customer.eat();
}
