/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   socket_template.hpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/17 14:50:14 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>

class Socket
{
	public:
		Socket(int domain, int service, int protocol, int port, u_long interface);
		virtual ~Socket() = default;

		virtual int			establish_connection(int sock, struct sockaddr_in address) = 0;
		void				test_status(int what);

		struct sockaddr_in	get_address();
		int					get_sock();
		int					get_connection();

		void				set_connection(int con);
		
	private:
		struct sockaddr_in	address;
		int					sock;
		int					connection;

	//hidden and unwanted
		Socket() = default;
};

#endif