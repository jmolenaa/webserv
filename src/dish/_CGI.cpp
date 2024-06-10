#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>
#include <ctime>

#include "log.hpp"
#include "dish.hpp"

Dish::CGI::CGI(Order& order, Status& status) : _order(order), _status(status)
{
	try
	{
		size_t	qpos = _order.getPath().find('?');
		if (qpos == std::string::npos)
		{
			_path = _order.getPath().substr(1);
			_query = _order.getBody();
		}
		else
		{
			_path = _order.getPath().substr(1, qpos);
			_query = _order.getPath().substr(qpos + 1);
		}
	}
	catch (...) {
		throw WebservException("Critical Error: " + std::string(std::strerror(errno)) + "\n");
	}
}

void Dish::CGI::_setEnv()
{
	switch (_order.getMethod())
	{
		case GET:
			_env[0] = const_cast<char*>("REQUEST_METHOD=GET");
			break;
		case POST:
			_env[0] = const_cast<char*>("REQUEST_METHOD=POST");
			break;
		case DELETE:
			_env[0] = const_cast<char*>("REQUEST_METHOD=DELETE");
			break;
		default:
			throw WebservException("Something real bad went down");
	}
	std::string string = "CONTENT_LENGTH=" + std::to_string(_order.getLength());
	_env[1] = const_cast<char*>(string.c_str());
	string = "QUERY_STRING=" + _query;
	_env[2] = const_cast<char*>(string.c_str());
	_env[3] = nullptr;
}

int Dish::CGI::execute()
{
	pipe(_inFD);
	pipe(_outFD);

	_pid = fork();
    if (_pid < 0)
        throw WebservException("Fork broke: " + std::string(std::strerror(errno)) + "\n");
    
    if (_pid == 0)
	{
       	_execChild();
		_execError("This should NEVER happen.", std::string(std::strerror(errno)));
	}

	close(_outFD[1]);
	close(_inFD[0]);
	write(_inFD[1], _order.getBody().c_str(), _order.getLength());
	close(_inFD[1]);
	waitpid(_pid, NULL, 0);
	return (_outFD[0]);
}

void Dish::CGI::_execError(std::string what, std::string why)
{
	close(_inFD[0]);
	close(_inFD[1]);
	close(_outFD[0]);
    close(_outFD[1]);
	_status.updateState(INTERNALERR);
    throw WebservException(what + why + "\n");
}

void Dish::CGI::_execChild()
{
	_setEnv();
	if (dup2(_outFD[1], STDOUT_FILENO) < 0)
		_execError("Couldn't dup2 output", std::string(std::strerror(errno)));
    if (dup2(_inFD[0], STDIN_FILENO) < 0)
		_execError("Couldn't dup2 input", std::string(std::strerror(errno)));
    
	close(_inFD[0]);
	close(_inFD[1]);
	close(_outFD[0]);
    close(_outFD[1]);

    char* path = const_cast<char*>(_path.c_str());
	char* argv[] = {path, path, nullptr};
    if (execve(path, argv, _env) < 0)
       _execError("execve failed: ", std::string(std::strerror(errno)));
}
