/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _serverHandlers.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:16:25 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/16 09:18:01 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/**
 * Accepts new connection on socket and adds to epoll instance
 * @todo use epoll class
*/
void Server::handleNewConnection() 
{
	sockaddr_in clientAddr{};
	socklen_t clientAddrLen = sizeof(clientAddr);
	int clientFd = accept(_serverfd, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
	if (clientFd == -1)
	{
		std::cerr << "Failed to accept connection:" << std::strerror(errno) << std::endl;
		return ;
	}
	_clientfds.insert(clientFd);

	_epoll.addFd(clientFd, EPOLLIN);
	// epoll_event event{};
	// event.events = EPOLLIN;
	// event.data.fd = clientFd;
	// if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, clientFd, &event) == -1)
	// 	std::cerr << "Failed to add to epoll: " << std::strerror(errno) << std::endl;
}
