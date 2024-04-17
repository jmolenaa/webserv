/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:10:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/17 16:55:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
	// Init address struct
	_address.sin_family = AF_INET6;
	_address.sin_port = htons(8080);
	_address.sin_addr.s_addr = INADDR_ANY;

	// Hardcoded connection limit
	_limit = 5;

	// Establish sockets
	_socket = socket(AF_INET6, SOCK_STREAM, 0);
	test_status(_socket);

	_connection = bind(_socket, (struct sockaddr *)&address, sizeof(address));
	test_status(_connection);

	listen(_socket, _limit);
};

Server::~Server()
{
	close(_socket);
}


// Checks if WHAT is < 0
void Server::test_status(int what)
{
	if (what < 0)
	{
		perror("Connection failed...");
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in Server::get_address()
{
	return _address;
};

int Server::get_sock()
{
	return _socket;
}

int Server::get_connection()
{
	return _connection;
}
