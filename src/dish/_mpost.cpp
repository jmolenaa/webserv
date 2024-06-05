/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _mpost.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 13:45:24 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/05 18:30:54 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

#include "log.hpp"
#include "dish.hpp"

/**
 * @todo USE EPOLL
*/
void Dish::_post(Order& order)
{
	if (order.getPath() != "/submit.cgi")
		return (_status.updateState(FORBIDDEN));

	_postExecCGI();
}

void Dish::_postExecCGI()
{
	pid_t	cpid;
	std::string filepath = "root/orders/" + _getDateTime();
	int fd = open(filepath.c_str(), O_RDWR | O_APPEND | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);

	cpid = fork();
	if (cpid == -1)
		throw WebservException("Fork broke: " + std::string(std::strerror(errno)) + "\n");
	
	if (cpid == 0)
		_postCGIChild(fd);
	waitpid(-1, NULL, 0);
	close(fd);
	_readFile(filepath.c_str());
}

void Dish::_postCGIChild(int fd)
{
	if (dup2(fd, STDOUT_FILENO) < 0)
			throw WebservException("Dup2 broke: " + std::string(std::strerror(errno)) + "\n");
	close(fd);

	char arg0[] = "/usr/bin/python3"; 
	char arg1[] = "root/cgi/submit.cgi";
	char *argv[] = {arg0, arg1, NULL};
	if (execve(arg0, argv, environ) == -1)
		throw WebservException("execve broke: " + std::string(std::strerror(errno)) + "\n");
}
