/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _mdelete.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 13:48:44 by dliu          #+#    #+#                 */
/*   Updated: 2024/06/05 18:40:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

#include "log.hpp"
#include "dish.hpp"

void Dish::_delete(Order& order)
{
	if (order.getPath() != "/delete.cgi")
		return (_status.updateState(FORBIDDEN));

	_deleteExecCGI();
}

void Dish::_deleteExecCGI()
{
	pid_t	cpid;
	std::string filepath = "root/tmpdel/" + _getDateTime();
	int fd = open(filepath.c_str(), O_RDWR | O_APPEND | O_TMPFILE);

	cpid = fork();
	if (cpid == -1)
		throw WebservException("Fork broke: " + std::string(std::strerror(errno)) + "\n");
	
	if (cpid == 0)
		_deleteCGIChild(fd);
	waitpid(-1, NULL, 0);
	close(fd);
	_readFile(filepath.c_str());
}

void Dish::_deleteCGIChild(int fd)
{
	if (dup2(fd, STDOUT_FILENO) < 0)
			throw WebservException("Dup2 broke: " + std::string(std::strerror(errno)) + "\n");
	close(fd);

	char arg0[] = "/usr/bin/python3"; 
	char arg1[] = "root/cgi/delete.cgi";
	char *argv[] = {arg0, arg1, NULL};
	if (execve(arg0, argv, environ) == -1)
		throw WebservException("execve broke: " + std::string(std::strerror(errno)) + "\n");
}
