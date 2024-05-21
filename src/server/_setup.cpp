/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _setup.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:34 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/21 14:15:14 by yizhang       ########   odam.nl         */
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
