/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket_template.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:42 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/17 14:20:28 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket_template.hpp"

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
	// Define address structure
	address.sin_family = domain;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = htonl(interface);

	// Establish socket
	sock = socket(domain, service, protocol);
	test_status(sock);
}

// Checks if WHAT is < 0
void Socket::test_status(int what)
{
	if (what < 0)
	{
		perror("Connection failed...");
		exit(EXIT_FAILURE);
	}
}

struct sockaddr_in Socket::get_address()
{
	return address;
};

int Socket::get_sock()
{
	return sock;
}

int Socket::get_connection()
{
	return connection;
}

void Socket::set_connection(int con)
{
	connection = con;
}