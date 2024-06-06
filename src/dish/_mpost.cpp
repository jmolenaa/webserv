/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _mpost.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 13:45:24 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/06 21:41:42 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

#include "log.hpp"
#include "dish.hpp"

void Dish::_post(Order& order)
{
	if (order.getPath() != "/submit.cgi")
		return (_status.updateState(FORBIDDEN));

	_postExecCGI(order.getBody());
}

/**
 * @todo needs to go through epoll
*/
void Dish::_postExecCGI(std::string data)
{
	pid_t	pid;
	std::string filepath = "root/posts/" + _getDateTime();
	int fd = open(filepath.c_str(), O_RDWR | O_APPEND | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

	pid = fork();
	if (pid < 0)
		throw WebservException("Fork broke: " + std::string(std::strerror(errno)) + "\n");
	
	if (pid == 0)
		_postCGIChild(fd, data);
	close(fd);
	waitpid(-1, NULL, 0);
	_readFile(filepath.c_str());
}

void Dish::_postCGIChild(int fd, std::string data)
{
	if (dup2(fd, STDOUT_FILENO) < 0)
			throw WebservException("Dup2 broke: " + std::string(std::strerror(errno)) + "\n");
	close(fd);

	char* arg0 = const_cast<char*>("root/cgi/submit.cgi");
	char* arg1 = const_cast<char*>(data.c_str());
	char *argv[] = {arg0, arg1, nullptr};
	if (execve(arg0, argv, nullptr) == -1)
		throw WebservException("execve broke: " + std::string(std::strerror(errno)) + "\n");
}

