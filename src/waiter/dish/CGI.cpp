/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CGI.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jmolenaa <jmolenaa@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 16:10:18 by jmolenaa      #+#    #+#                 */
/*   Updated: 2024/06/28 16:10:18 by jmolenaa      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "dish.hpp"

#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>
#include <ctime>

#include "webservException.hpp"
#include "log.hpp"
#include "restaurant.hpp"
#include "customer.hpp"

CGI::CGI(Dish& parent) : FdHandler(parent.restaurant), _dish(parent), _path(parent.finalPage), _CGIInputPipe{-1, -1}, _CGIOutputPipe{-1, -1},
						_pid(-1), _pos(0), _message(_dish.order.getOrder()), _buffer(""), _env(nullptr)
{
	try
	{
		size_t	qpos = this->_path.find('?');
		if (qpos == std::string::npos)
		{
			if (_dish.order.getMethod() == POST)
				_query = _dish.order.getBody();
		}
		else
		{
			_query = this->_path.substr(qpos + 1);
			this->_path = this->_path.substr(0, qpos);
		}
		_setEnv();
	}
	catch (...) {
		throw WebservException("Critical Error: " + std::string(std::strerror(errno)) + "\n");
	}
}

CGI::~CGI()
{
	if (this->_inFD != -1) {
		restaurant.removeFdHandler(this->_inFD);
	}
	if (this->_outFD != -1) {
		restaurant.removeFdHandler(this->_outFD);
	}
	close(_CGIOutputPipe[0]);
	close(_CGIOutputPipe[1]);
	close(_CGIInputPipe[0]);
	close(_CGIInputPipe[1]);
	for (size_t i = 0; _env && _env[i]; ++i)
        delete[] _env[i];
    delete[] _env;
}

//this probably needs to be updated
void CGI::_setEnv()
{
	switch (_dish.order.getMethod())
	{
		case GET:
			_vec.push_back("REQUEST_METHOD=GET");
			break;
		case POST:
			_vec.push_back("REQUEST_METHOD=POST");
			break;
		case DELETE:
			_vec.push_back("REQUEST_METHOD=DELETE");
			break;
		default:
			throw WebservException("Something real bad went down");
	}
	_vec.push_back("CONTENT_TYPE=" + _dish.order.getType());
	_vec.push_back("CONTENT_LENGTH=" + std::to_string(_dish.order.getLength()));
	_vec.push_back("QUERY_STRING=" + _query);
	_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_vec.push_back("SCRIPT_NAME=" + _path);
	_vec.push_back("UPLOAD_DIR=" + _dish.recipe.uploadDir);
	_env = new char*[_vec.size() + 1];

	 for (size_t i = 0; i < _vec.size(); i++)
	 {
	 	_env[i] = new char[_vec[i].length() + 1];
	 	std::strcpy(_env[i], _vec[i].c_str());
	 	Log::getInstance().print(std::string(_env[i]));
	 }
	 _env[_vec.size()] = nullptr;
}

bool CGI::_thereIsAnInitialError() {
	if (this->_dish.status.getState() != OK) {
		return true;
	}
	else if (this->_dish.recipe.allowCgi == false) {
		this->_dish.status.updateState(FORBIDDEN);
	}
	else if (access(this->_path.c_str(), F_OK) == -1) {
		this->_dish.status.updateState(NOTFOUND);
	}
	else if (access(this->_path.c_str(), X_OK) == -1) {
		this->_dish.status.updateState(FORBIDDEN);
	}
	else if (!this->_dish.isRightCGIExtenion(this->_path)) {
		this->_dish.status.updateState(FORBIDDEN);
	}
	else {
		return false;
	}
	return true;
}

void CGI::execute()
{
	if (_thereIsAnInitialError()) {
		return ;
	}
	if (pipe(_CGIInputPipe) == -1) {
		this->_closePipes("CGI pipe function failed: ", std::string(std::strerror(errno)) + "\n");
	}
	if (pipe(_CGIOutputPipe) == -1) {
		this->_closePipes("CGI pipe function failed: ", std::string(std::strerror(errno)) + "\n");
	}
	this->_inFD = _CGIOutputPipe[0];
	this->_outFD = _CGIInputPipe[1];

	restaurant.addFdHandler(_inFD, this, EPOLLIN | EPOLLHUP | EPOLLERR );
	restaurant.addFdHandler(_outFD, this, EPOLLOUT);

	_pid = fork();
    if (_pid < 0)
		_closePipes("Fork broke: ", std::string(std::strerror(errno)) + "\n");

    if (_pid == 0)
	{
       	_execChild();
	}

	close(_CGIOutputPipe[1]);
	close(_CGIInputPipe[0]);
}

void CGI::_closePipes(std::string what, std::string why)
{
	close(_CGIInputPipe[0]);
	close(_CGIInputPipe[1]);
	close(_CGIOutputPipe[0]);
    close(_CGIOutputPipe[1]);
	if (!what.empty())
	{
		Log::getInstance().printErr(what + why);
		_dish.status.updateState(INTERNALERR);
		_dish.doError();
	}
}

void CGI::_execChild()
{
	if (dup2(_CGIOutputPipe[1], STDOUT_FILENO) < 0)
		_closePipes("Couldn't dup2 output: ", std::string(std::strerror(errno)));
    if (dup2(_CGIInputPipe[0], STDIN_FILENO) < 0)
		_closePipes("Couldn't dup2 input: ", std::string(std::strerror(errno)));
    
	_closePipes("", "");

	char* path = const_cast<char*>(_path.c_str());
	char* argv[] = {path, nullptr};
	Log::getInstance().printErr("Executing CGI for path " + _path);
    if (execve(path, argv, _env) < 0)
       _closePipes("execve failed: ", std::string(std::strerror(errno)));
	exit(EXIT_FAILURE);
}

void	CGI::input(int eventFD)
{
	if (eventFD != _inFD)
       _closePipes("Bad FD triggered in CGI input ", std::to_string(eventFD));

	ssize_t count = read(_inFD, _buffer, BUF_LIMIT - 1);
	if (count < 0)
	{
		if (_dish.status.getState() != OK) {
			_dish.status.updateState(COUNT);
		}
		else {	
			_dish.status.updateState(INTERNALERR);
		}
		_closePipes("Read error in CGI input!", std::string(std::strerror(errno)));
		return ;
	}
	else if (count < BUF_LIMIT - 1)
	{
		_dish.body.append(_buffer, count);
		Log::getInstance().print("Finished cooking CGI " + std::to_string(_inFD) + "!");
		restaurant.removeFdHandler(this->_inFD);
		this->_inFD = -1;
		close(this->_CGIOutputPipe[0]);
		this->_dish.customer.eat();
	}
	else
	{
		Log::getInstance().print("CGI is cooking " + std::to_string(count) + " ingredients");
		_dish.body.append(_buffer, count);// += std::string(_buffer);
	}
}

void	CGI::output(int eventFD)
{
	if (eventFD != _outFD)
       _closePipes("Bad FD triggered in CGI output ", std::to_string(eventFD));

	Log::getInstance().print("Adding ingredients from position " + std::to_string(_pos));
	std::string msg = _message.substr(_pos, BUF_LIMIT - 1);
	ssize_t count = write(_outFD, msg.c_str(), msg.size());
	if (count < 0)
	{
		if (_dish.status.getState() != OK) {
			_dish.status.updateState(COUNT);
		}
		else {	
			_dish.status.updateState(INTERNALERR);
		}
		_closePipes("Write error in Dish output!", std::string(std::strerror(errno)));
		return ;
	}
	else
	{
		_pos += count;
		if (_pos >= _message.size())
		{
			restaurant.removeFdHandler(this->_outFD);
			this->_outFD = -1;
			close(this->_CGIInputPipe[1]);
		}
	}
}