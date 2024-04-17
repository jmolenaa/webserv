/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket_server.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:10:16 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/17 14:21:58 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "socket_server.hpp"

ServerSocket::ServerSocket(int domain, int service, int protocol, int port, u_long interface)
	: Socket(domain, service, protocol, port, interface)
{
	set_connection(establish_connection(get_sock(), get_address()));
	test_status(get_connection());
};

int ServerSocket::establish_connection(int sock, struct sockaddr_in address)
{
	return (bind(sock, (struct sockaddr *)&address, sizeof(address)));
}