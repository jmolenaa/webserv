/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket_server.hpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/17 14:50:25 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_SERVER_HPP
# define SOCKET_SERVER_HPP

# include "socket_template.hpp"

class ServerSocket: public Socket
{
	public:
		ServerSocket(int domain, int service, int protocol, int port, u_long interface);
		~ServerSocket() = default;

		int		establish_connection(int sock, struct sockaddr_in address) override;
};

#endif