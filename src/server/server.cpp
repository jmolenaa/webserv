/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:49 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 13:12:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "log.hpp"

Server::Server(Epoll& epoll, ServerConfigs configs)
	: _epoll(epoll), _configs(configs)
{
	Log::getInstance().print("Server constrycted with " + std::to_string(configs.size()) + " configurations.");

	_createSocket();
	_bindToAddress();
    _epoll.addFd(_serverfd, EPOLLIN);

	if (listen(_serverfd, SOMAXCONN) == -1)
		throw WebservException("Server could not listen because: " + std::string(std::strerror(errno)) + "\n");
}

Server::~Server()
{
    if (_serverfd > 0)
        close(_serverfd);
}

void Server::run()
{
	Log::getInstance().print("Server with " + std::to_string(_configs.size()) + " configurations is running");
    epoll_event events[CLI_LIMIT];
    while (true)
	{
        _epoll.wait_events(-1, events);
        _handleEvents(events, _epoll.getNumEvents());
    }    
}

void Server::_handleEvents(epoll_event* events, int numEvents)
{
    for (int i = 0; i < numEvents; i++)
	{
        if (events[i].data.fd == _serverfd)
            _handleNewConnection();
        else
            _handleClientRequest(events[i].data.fd);
    }
}
