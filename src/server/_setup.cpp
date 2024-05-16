/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _setup.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:34 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/16 09:32:49 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"

/**
 * Creates servers socket fd using socket()
 * exits() on failure to do so
*/ 
void	Server::createSocket()
{
	_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverfd < 0)
	{
		std::cerr << "Failed to create socket: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Binds socketfd to sockaddr_in
 * exits() on failure to do so
*/
void	Server::bindToAddress()
{
	int	optvalTrue = 1;
	setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR, &optvalTrue, sizeof(optvalTrue));
	setsockopt(_serverfd, SOL_SOCKET, SO_REUSEPORT, &optvalTrue, sizeof(optvalTrue));
	sockaddr_in	serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_serverfd, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1)
	{
		std::cerr << "Failed to bind to socket: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}

/**
 * Sets up epoll instance, and adds _serverfd to it
 * @todo remove from server class and make epoll class with public methods to add fds to the epoll instance
 * exits() upon failure to do these
*/
void	Server::setupEpoll()
{
	Epoll _epoll = Epoll();
	// _epollfd = epoll_create(CLI_LIMIT);
	// if (_epollfd < 0)
	// {
	// 	std::cerr << "Failed to create epoll instance:" << std::strerror(errno) << std::endl;
	// 	exit(EXIT_FAILURE);
	// }

	_epoll.addFd(_serverfd, EPOLLIN);
	// epoll_event event{};
	// event.events = EPOLLIN;
	// event.data.fd = _serverfd;
	// if (epoll_ctl(_epoll.getEpollFd(), EPOLL_CTL_ADD, _serverfd, &event) == -1)
	// {
	// 	std::cerr << "Failed to add to epoll: " << std::strerror(errno) << std::endl;
	// 	exit(EXIT_FAILURE);
	// }
}
