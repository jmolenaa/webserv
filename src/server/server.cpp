/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/21 14:14:56 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server(Epoll& epoll) : _epoll(epoll), _port(htons(PORT)), _address(htonl(INADDR_ANY)), _name("localhost"), _locations({Location()})
{
	createSocket();
	bindToAddress();
    _epoll.addFd(_serverfd, EPOLLIN);
    
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
        _epoll.wait_events(-1, events);
        handleEvents(events, _epoll.getNumEvents());
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
