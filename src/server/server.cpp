/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/25 17:20:01 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/**
 * @todo remove setupEpoll from here and put it in its own class.
*/
Server::Server()
{
	createSocket();
	bindToAddress();
	setupEpoll();

	if (listen(_serverfd, SOMAXCONN) == -1)
	{
		std::cerr << "Failed to listen: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);	
	}
}

Server::~Server()
{
    if (_serverfd > 0)
        close(_serverfd);
    if (_epollfd > 0)
        close(_epollfd);
    for (int fd : _clientfds) 
	{
        if (fd > 0)
            close(fd);
    }
}

void Server::run()
{
    epoll_event events[CLI_LIMIT];
    while (true)
	{
        int numEvents = epoll_wait(_epollfd, events, CLI_LIMIT, -1);
        if (numEvents == -1) {
            std::cerr << "Epoll failed to wait: " << std::strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
        handleEvents(events, numEvents);
    }    
}

void Server::handleEvents(epoll_event* events, int numEvents)
{
    for (int i = 0; i < numEvents; i++)
	{
        if (events[i].data.fd == _serverfd)
            handleNewConnection();
        else
            handleClientRequest(events[i].data.fd);
    }
}
