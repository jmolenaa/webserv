/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/17 20:01:12 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <unistd.h>

class Server
{
	public:
		public:
		Server();
		~Server() = default;

		void				test_status(int what);
		void				listening(int what, );
		
		struct sockaddr_in	get_address();
		int					get_sock();
		int					get_connection();
		
	private:
		struct sockaddr_in	_address;
		int					_socket;
		int					_connection;
		int					_limit;
};

#endif