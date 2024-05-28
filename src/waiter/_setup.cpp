/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _setup.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:19:34 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/28 12:44:42 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

# include "waiter.hpp"

/**
 * Creates waiters socket fd using socket()
*/ 
void	Waiter::_createSocket()
{
	_waiterFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_waiterFd < 0)
		throw WebservException("Failed to create socket because: " + std::string(std::strerror(errno)) + "\n");
}

/**
 * Binds socketfd to sockaddr_in
*/
void	Waiter::_bindToAddress()
{
	int	optvalTrue = 1;
	setsockopt(_waiterFd, SOL_SOCKET, SO_REUSEADDR, &optvalTrue, sizeof(optvalTrue));
	setsockopt(_waiterFd, SOL_SOCKET, SO_REUSEPORT, &optvalTrue, sizeof(optvalTrue));
	sockaddr_in	waiterAddr{};
	waiterAddr.sin_family = AF_INET;
	Log::getInstance().print("Waiter is waiting at table " + std::to_string(ntohs(_kitchen.begin()->second.getTable())));
	waiterAddr.sin_port = _kitchen.begin()->second.getTable();
	waiterAddr.sin_addr.s_addr = _kitchen.begin()->second.getAddress();
	if (bind(_waiterFd, reinterpret_cast<sockaddr *>(&waiterAddr), sizeof(waiterAddr)) == -1)
		throw WebservException("Failed to bind to socket because: " + std::string(std::strerror(errno)) + "\n");
}
