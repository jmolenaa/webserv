#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>
#include <ctime>

#include "log.hpp"
#include "dish.hpp"
#include "webservException.hpp"
#include "order.hpp"

Dish::CGI::CGI(Dish& parent) : _dish(parent)
{
	try
	{
		size_t	qpos = _dish._order.getPath().find('?');
		if (qpos == std::string::npos)
		{
			_path = _dish._order.getPath().substr(1);
			_query = _dish._order.getBody();
		}
		else
		{
			_path = _dish._order.getPath().substr(1, qpos);
			_query = _dish._order.getPath().substr(qpos + 1);
		}
		_setEnv();
	}
	catch (...) {
		throw WebservException("Critical Error: " + std::string(std::strerror(errno)) + "\n");
	}
}

void Dish::CGI::_setEnv()
{
	switch (_dish._order.getMethod())
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
	_vec.push_back("CONTENT_TYPE=" + _dish._order.getType());
	_vec.push_back("CONTENT_LENGTH=" + std::to_string(_dish._order.getLength()));
	_vec.push_back("QUERY_STRING=" + _query);
	_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_vec.push_back("SCRIPT_NAME=" + _path);
	_vec.push_back("UPLOAD_DIR=" + _dish._recipe.uploadDir);
	_env = new char*[_vec.size()];

	for (size_t i = 0; i < _vec.size(); i++)
	{
		_env[i] = new char[_vec[i].length() + 1];
		std::strcpy(_env[i], _vec[i].c_str());
		Log::getInstance().print(std::string(_env[i]));
	}
}

void Dish::CGI::_freeEnv()
{
	for (size_t i = 0; i < _vec.size(); ++i)
        delete[] _env[i];
 
     delete[] _env;
}

/**
 * @todo time out CGIs, add fd to epoll
 */
int Dish::CGI::execute()
{
	pipe(_inFD);
	pipe(_outFD);

	_pid = fork();
    if (_pid < 0)
		_execError("Fork broke: ", std::string(std::strerror(errno)) + "\n");
    
    if (_pid == 0)
	{
       	_execChild();
		_execError("This should NEVER happen.", std::string(std::strerror(errno)));
	}

	close(_outFD[1]);
	close(_inFD[0]);
	write(_inFD[1], _dish._order.getOrder().c_str(), _dish._order.getOrder().size());
	close(_inFD[1]);
	waitpid(_pid, NULL, 0);
	_freeEnv();
	return (_outFD[0]);
}

void Dish::CGI::_execError(std::string what, std::string why)
{
	close(_inFD[0]);
	close(_inFD[1]);
	close(_outFD[0]);
    close(_outFD[1]);
	_freeEnv();
	_dish._status.updateState(INTERNALERR);
    throw WebservException(what + why + "\n");
}

void Dish::CGI::_execChild()
{
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
