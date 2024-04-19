/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   _setup.cpp										 :+:	:+:			*/
/*													 +:+					*/
/*   By: dliu <dliu@student.codam.nl>				 +#+					 */
/*												   +#+					  */
/*   Created: 2024/04/19 16:54:58 by dliu		  #+#	#+#				 */
/*   Updated: 2024/04/19 16:59:11 by dliu		  ########   odam.nl		 */
/*																			*/
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
}

/**
 * Sets up epoll instance, and adds _serverfd to it
 * exits() upon failure to do these
*/
void	Server::setupEpoll()
{
	_epollfd = epoll_create(CLI_LIMIT);
	if (_epollfd < 0)
	{
		std::cerr << "Failed to create epoll instance:" << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	epoll_event event{};
	event.events = EPOLLIN;
	event.data.fd = _serverfd;
	if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _serverfd, &event) == -1) {
		std::cerr << "Failed to add to epoll: " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
}
