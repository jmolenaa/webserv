#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <fcntl.h>
#include <ctime>

#include "log.hpp"
#include "dish.hpp"

/**
 * @todo needs to go through epoll
 */
Dish::CGI::CGI(Order& order) : _order(order)
{
	if (_order.getMethod() == POST)
		_cgiPath = "cgi/post.cgi";
	else if (_order.getMethod() == DELETE)
		_cgiPath = "cgi/delete.cgi";
	else
		throw WebservException("Wtf\n");
	_filename = "orderlog/" + _generateFilename();
	_fd = open(_filename.c_str(), O_RDWR | O_APPEND | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (_fd < 0)
        throw WebservException("Failed to create new file: " + std::string(std::strerror(errno)) + "\n");
}

std::string Dish::CGI::execute()
{
    pid_t pid = fork();
    if (pid < 0)
        throw WebservException("Fork broke: " + std::string(std::strerror(errno)) + "\n");
    
    if (pid == 0) {
       	_execChild();
	}

	close(_fd);
	waitpid(pid, NULL, 0);
	return (_filename);
}

std::string	Dish::CGI::_generateFilename()
{
    std::time_t now = std::time(nullptr);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%H%M%S", std::localtime(&now));
    return std::string(buffer);
}

void Dish::CGI::_execChild()
{
	if (dup2(_fd, STDOUT_FILENO) < 0) {
        close(_fd);
        throw WebservException("Couldn't dup2 stdout: " + std::string(std::strerror(errno)) + "\n");
    }
    close(_fd);

    // _setEnv();

    char* arg0 = const_cast<char*>(_cgiPath.c_str());
	std::string arg = _order.getBody();
	char* arg1 = const_cast<char*>(arg.c_str());
	char* argv[] = {arg0, arg1, nullptr};

    if (execve(arg0, argv, nullptr) < 0)
        throw WebservException("execve failed: " + std::string(std::strerror(errno)) + "\n");
}

// void Dish::CGI::_setEnv()
// {
// 	switch (_order.getMethod())
// 	{
// 		case POST:
// 			setenv("REQUEST_METHOD", "POST", 1);
// 			break;
// 		case DELETE:
// 			setenv("REQUEST_METHOD", "DELETE", 1);
// 			break;
// 		default:
// 			throw WebservException("Something real bad went down");
// 	}
// 	setenv("CONTENT_LENGTH", std::to_string(_order.getLength()).c_str(), 1);
// 	setenv("CONTENT_TYPE", _order.getType().c_str(), 1);
// }
