/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _setup.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:34 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/24 13:03:39 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "server.hpp"

/**
 * Creates servers socket fd using socket()
*/ 
void	Server::_createSocket()
{
	_serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverfd < 0)
		throw WebservException("Failed to create socket because: " + std::string(std::strerror(errno)) + "\n");
}

/**
 * Binds socketfd to sockaddr_in
 * @todo make it use its _config
*/
void	Server::_bindToAddress()
{
	int	optvalTrue = 1;
	setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR, &optvalTrue, sizeof(optvalTrue));
	setsockopt(_serverfd, SOL_SOCKET, SO_REUSEPORT, &optvalTrue, sizeof(optvalTrue));
	sockaddr_in	serverAddr{};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(_serverfd, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1)
		throw WebservException("Failed to bind to socket because: " + std::string(std::strerror(errno)) + "\n");
}
