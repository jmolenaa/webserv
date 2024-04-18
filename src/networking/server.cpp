/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:10:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/18 17:58:08 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() : _epoll()
{
	_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverfd < 0)
	{
		std::cerr << "Failed to create socket: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	sockaddr_in	serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_serverfd, reinterpret_cast<sockaddr *>(&serverAddr), 
		sizeof(serverAddr)) == -1)
	{
		std::cerr << "Failed to bind to socket: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(_serverfd, SOMAXCONN) == -1)
	{
		std::cerr << "Failed to listen: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);	
	}
	
	_epoll.add(_serverfd);
};

Server::~Server()
{
	if (_serverfd > 0)
		close(_serverfd);
}

void Server::run()
{
	epoll_event events[CLI_LIMIT];
	while (true)
	{
		int numEvents = _epoll.wait(events);
		for (int i = 0; i < numEvents; i++)
		{
			if (events[i].data.fd == _serverfd)
			{
				sockaddr_in		clientAddr{};
				socklen_t		clientAddrLen = sizeof(clientAddr);
				int clientFd	= accept(_serverfd, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
				if (clientFd == -1)
				{
					std::cerr << "Failed to accept connection:" << std::strerror(errno) << std::endl;
					continue;
				}
				_epoll.add(clientFd);
				serveClient(clientFd, "You've been served\n");
			}
			else
			{
				char 	buffer[1024];
				ssize_t	numBytes = recv(events[i].data.fd, buffer, sizeof(buffer) - 1, 0);
				if (numBytes <= 0)
				{
					close(events[i].data.fd);
					continue;
				}
				buffer[numBytes] = '\0';
				std::cout << "Received: " << buffer << std::endl;
			}
		}
	}	
}

void	Server::serveClient(int clientFd, const std::string& message)
{
	ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
	if (bytesSent == -1)
		std::cerr << "Failed to send message: " << std::strerror(errno) << std::endl;
	// else
	// 	std::cout << "Sent message: " << message << std::endl;
}
