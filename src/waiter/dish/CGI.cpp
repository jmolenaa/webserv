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

CGI::CGI(Dish& parent) : FdHandler(parent.restaurant, CGITYPE), _dish(parent), _path(parent.finalPage), _CGIInputPipe{-1, -1}, _CGIOutputPipe{-1, -1},
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
	this->_closePipes();
	for (size_t i = 0; _env && _env[i]; ++i)
        delete[] _env[i];
    delete[] _env;

	// Kill the child process if it's still running
	if (waitpid(this->_pid, nullptr, WNOHANG) == 0) {
		kill(this->_pid, SIGQUIT);
	}
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
	if (pipe(_CGIInputPipe) == -1 || pipe(_CGIOutputPipe) == -1) {
		this->_CGIError("CGI pipe function failed: ", std::string(std::strerror(errno)) + "\n");
		return ;
	}
	this->_inFD = _CGIOutputPipe[0];
	this->_outFD = _CGIInputPipe[1];


	_pid = fork();
    if (_pid < 0) {
		this->_CGIError("Fork broke: ", std::string(std::strerror(errno)) + "\n");
		return ;
	}
    if (_pid == 0) {
       	_execChild();
	}

	restaurant.addFdHandler(_inFD, this, EPOLLIN | EPOLLHUP | EPOLLERR );
	restaurant.addFdHandler(_outFD, this, EPOLLOUT);

	close(_CGIOutputPipe[1]);
	close(_CGIInputPipe[0]);
}

void CGI::_CGIError(std::string what, std::string why) {
	this->_closePipes();
	Log::getInstance().printErr(what + why);
	_dish.status.updateState(INTERNALERR);
	_dish.doError();
}

static void	safeClose(int& fd) {
	if (fd != -1) {
		close(fd);
		fd = -1;
	}
}

void CGI::_closePipes()
{
	safeClose(_CGIInputPipe[0]);
	safeClose(_CGIInputPipe[1]);
	safeClose(_CGIOutputPipe[0]);
    safeClose(_CGIOutputPipe[1]);
}

void CGI::_tryChangeDir() {
	std::string	parentDir;
	size_t		pos;

	pos = this->_path.find_last_of('/');
	if (pos != std::string::npos) {
		parentDir = this->_path.substr(0, pos);
		if (chdir(parentDir.c_str()) == 0) {
			return;
		}
		Log::getInstance().printErr("Couldn't change directory " + std::string(std::strerror(errno)));
	}
	else {
		Log::getInstance().printErr("Something's horribly wrong with the path given.");
	}
	this->_closePipes();
	exit(INTERNALERR);
}

void CGI::_execChild()
{
	if (dup2(_CGIOutputPipe[1], STDOUT_FILENO) < 0 || dup2(_CGIInputPipe[0], STDIN_FILENO) < 0) {
		this->_closePipes();
		Log::getInstance().printErr("Couldn't dup2 output: " + std::string(std::strerror(errno)));
		exit (INTERNALERR);
	}

	_closePipes();
	this->_tryChangeDir();

	std::string	path = this->_path.substr(this->_path.find_last_of('/') + 1);
	char*		pathCStr = const_cast<char*>(path.c_str());
	char*	argv[] = {pathCStr, nullptr};
	Log::getInstance().printErr(std::string(RESET) + "Executing CGI for path " + _path);
    if (execve(argv[0], argv, _env) < 0) {
		this->_closePipes();
		Log::getInstance().printErr("execve failed: " + std::string(std::strerror(errno)));
	}
	exit (INTERNALERR);
}

void	CGI::input(int eventFD)
{
	this->_dish.customer.resetTime();
	if (eventFD != _inFD) {
		this->_CGIError("Bad FD triggered in CGI input ", std::to_string(eventFD));
	}

	ssize_t count = read(_inFD, _buffer, BUF_LIMIT);
	if (count < 0) {
		this->_CGIError("Read error in CGI input!", std::string(std::strerror(errno)));
	}
	else if (count == 0)
	{
		Log::getInstance().print("Finished cooking CGI " + std::to_string(_inFD) + "!");
		this->_removeHandler(this->_inFD);
		this->_dish.customer.eat();
	}
	else
	{
		Log::getInstance().print("CGI is cooking " + std::to_string(count) + " ingredients");
		_dish.body.append(_buffer, count);
//		if (_pos == _message.size()) {
//			Log::getInstance().print("Finished cooking CGI " + std::to_string(_inFD) + "!");
//			this->_removeHandler(this->_inFD);
//			this->_dish.customer.eat();
//		}
	}
}

void	CGI::output(int eventFD)
{
	this->_dish.customer.resetTime();
	if (eventFD != _outFD) {
		this->_CGIError("Bad FD triggered in CGI input ", std::to_string(eventFD));
	}

	Log::getInstance().print("Adding ingredients from position " + std::to_string(_pos));
	std::string msg = _message.substr(_pos, BUF_LIMIT);
	ssize_t count = write(_outFD, msg.c_str(), msg.size());
	if (count < 0) {
		this->_CGIError("Read error in CGI output!", std::string(std::strerror(errno)));
	}
	else if (count == 0) {
		this->_removeHandler(this->_outFD);
	}
	else {
		_pos += count;
	}
}

void CGI::_removeHandler(int &handlerFd) {
	if (handlerFd != -1) {
		restaurant.removeFdHandler(handlerFd);
		close(handlerFd);
		handlerFd = -1;
	}
}

void CGI::handleCGIHangup() {
	int	exitStatus;
	if (waitpid(this->_pid, &exitStatus, WNOHANG) == 0) {
		return ;
	}
	if (WIFEXITED(exitStatus) && WEXITSTATUS(exitStatus) < COUNT) {
		this->_dish.status.updateState((status)WEXITSTATUS(exitStatus));
	}
	else {
		this->_dish.status.updateState(INTERNALERR);
	}
	this->_removeHandler(this->_inFD);
	this->_removeHandler(this->_outFD);
	this->_closePipes();

	// if CGI exits normally but doesn't write anything
	if (this->_dish.status.getState() == OK && this->_dish.body.empty()) {
		this->_dish.body = "Script executed successfully\n";
		this->_dish.customer.eat();
	}
	else if (this->_dish.status.getState() == OK) {
		this->_dish.customer.eat();
	}
	else {
		this->_dish.doError();
	}
}

//pid_t CGI::getPid() const {
//	return this->_pid;
//}

//void CGI::inspectChildExitCode() {
//	int	exitStatus;
//
//	std::cout << waitpid(this->_pid, &exitStatus, WNOHANG) << " \n";
//	if (WIFEXITED(exitStatus) && WEXITSTATUS(exitStatus) < COUNT) {
//		this->_dish.status.updateState((status)WEXITSTATUS(exitStatus));
//	}
//	else {
//		this->_dish.status.updateState(INTERNALERR);
//	}
//	this->_removeHandler(this->_inFD);
//	this->_removeHandler(this->_outFD);
//	this->_closePipes();
//	if (this->_dish.status.getState() == OK && this->_dish.body.empty()) {
//		this->_dish.body = "Script executed successfully\n";
//	}
//}
