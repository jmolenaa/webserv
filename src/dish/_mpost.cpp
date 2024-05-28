/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _mpost.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/05/28 13:45:24 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/28 14:52:21 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <cstring>

#include "log.hpp"
#include "dish.hpp"

/**
 * @todo USE EPOLL
*/
void Dish::_post(Order& order)
{
	if (order.getPath() != "/submit.py")
		return (_status.updateState(FORBIDDEN));

	std::string postBody = order.getBody();
	
	//CGI stuff
	// int		pipefd[2];
	// pid_t	cpid;

	// if (pipe(pipefd) == -1)
	// 	throw WebservException("Pipe broke: " + std::string(std::strerror(errno)) + "\n");
	
	// cpid = fork();
	// if (cpid == -1)
	// 	throw WebservException("Fork broke: " + std::string(std::strerror(errno)) + "\n");
	
	// if (cpid == 0)
	// {
	// 	execve("root/submit.py", NULL, NULL);
	// }

	Log::getInstance().print("Posting: " + postBody);
	_body += "Posting: " + postBody;
}
	

