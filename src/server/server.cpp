/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/21 14:02:29 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/**
 * @todo remove setupEpoll from here and put it in its own class.
 * make it so the server only listens on a specified port
 * move the binding, epoll and listening to a different function, initialisation?, why did we not specify port and address?
*/
Server::Server() : _port(htons(PORT)), _address(htonl(INADDR_ANY)), _name("localhost"), _locations({Location()})
{
	createSocket();
	bindToAddress();
	//setupEpoll();
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
        // int numEvents = epoll_wait(_epollfd, events, CLI_LIMIT, -1);
        // if (numEvents == -1) {
        //     std::cerr << "Epoll failed to wait: " << std::strerror(errno) << std::endl;
        //     exit(EXIT_FAILURE);
        // }
        //handleEvents(events, numEvents);
    }    
}
//probably will need to be moved to Epoll class? Will likely need to rewrite
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
