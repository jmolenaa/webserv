/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/08 18:13:44 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

/**
 * @todo remove setupEpoll from here and put it in its own class.
 * make it so the server only listens on a specified port
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
    epoll_event events[CLI_LIMIT];//change it to vector
    
    while (true)
	{
        //*use Epoll.wait_events();
        
        int numEvents = epoll_wait(_epollfd, events, CLI_LIMIT, -1);
        if (numEvents == -1) {
            std::cerr << "Epoll failed to wait: " << std::strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }
        handleEvents(events, numEvents);
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
