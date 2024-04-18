/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:10:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/18 13:45:42 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server()
{
	_cli_pos = 0;
	_cli_count = 0;

	// Init address struct
	_address.sin_family = AF_INET;
	_address.sin_port = htons(8080);
	_address.sin_addr.s_addr = INADDR_ANY;

	// Establish sockets
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	test_status(_socket);

	_connection = bind(_socket, (struct sockaddr *)&_address, sizeof(_address));
	test_status(_connection);

	listen(_socket, CLI_LIMIT);
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

void Server::listening()
{
	_clients[_cli_pos] = accept(_socket, nullptr, nullptr);
	std::cout << "Welcoming client\n";
	recv(_clients[_cli_pos], _buffers[_cli_pos], BUF_LIMIT, 0);
	std::cout << "Message from client: " << _buffers[_cli_pos] << std::endl;
	char serve_msg[] = "You've been served\n";
	send(_clients[_cli_pos], serve_msg, std::strlen(serve_msg), 0);	
	close(_clients[_cli_pos]);
}

struct sockaddr_in Server::get_address()
{
	return _address;
}

int Server::get_sock()
{
	return _socket;
}

int Server::get_connection()
{
	return _connection;
}
