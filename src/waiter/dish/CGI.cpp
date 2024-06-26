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

CGI::CGI(Dish& parent) : FdHandler(parent.restaurant), _dish(parent), _pos(0), _message(_dish.order.getOrder()), _env(nullptr)
{
	try
	{
		size_t	qpos = _dish.order.getPath().find('?');
		if (qpos == std::string::npos)
		{
			_path = _dish.order.getPath().substr(1);
			if (_dish.order.getMethod() == POST)
				_query = _dish.order.getBody();
		}
		else
		{
			_path = _dish.order.getPath().substr(1, qpos);
			_query = _dish.order.getPath().substr(qpos + 1);
		}
		_setEnv();
	}
	catch (...) {
		throw WebservException("Critical Error: " + std::string(std::strerror(errno)) + "\n");
	}
}

CGI::~CGI()
{
	for (size_t i = 0; _env && _env[i]; ++i)
        delete[] _env[i];
 
    delete[] _env;
	_closePipes("", "");
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

/**
 * @todo time out CGIs
 */
void CGI::execute()
{
	pipe(_CGIInputPipe);
	pipe(_CGIOutputPipe);
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
		_closePipes("This should NEVER happen.", std::string(std::strerror(errno)));
	}

	close(_CGIOutputPipe[1]);
	close(_CGIInputPipe[0]);
	output(_outFD);
}

void CGI::_closePipes(std::string what, std::string why)
{
	close(_CGIInputPipe[0]);
	close(_CGIInputPipe[1]);
	close(_CGIOutputPipe[0]);
    close(_CGIOutputPipe[1]);
	close(_inFD);
	close(_outFD);
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
		_closePipes("Couldn't dup2 output", std::string(std::strerror(errno)));
    if (dup2(_CGIInputPipe[0], STDIN_FILENO) < 0)
		_closePipes("Couldn't dup2 input", std::string(std::strerror(errno)));
    
	_closePipes("", "");

	char* path = const_cast<char*>(_path.c_str());
	char* argv[] = {path, path, nullptr};
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
		_buffer[count] = '\0';
		_dish.body += std::string(_buffer);
		Log::getInstance().print("Finished cooking CGI " + std::to_string(_inFD) + "!");
		
		close(_inFD);
		_closePipes("", "");
		_dish.done = true;
		return;
	}
	else
	{
		Log::getInstance().print("CGI is cooking " + std::to_string(count) + " ingredients");
		_buffer[count] = '\0';
		_dish.body += std::string(_buffer);
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
			restaurant.removeFdHandler(_outFD);
			close(_outFD);
			waitpid(_pid, NULL, 0); //add timeout and error catching somewhere here
			input(_inFD);
		}
	}
}