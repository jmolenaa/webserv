/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/18 13:42:54 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <iostream>
# include <unistd.h>
# include <cstring>

# define CLI_LIMIT 5
# define BUF_LIMIT 1024

class Server
{
	public:
		public:
		Server();
		~Server();

		void				test_status(int what);
		void				listening();
		
		struct sockaddr_in	get_address();
		int					get_sock();
		int					get_connection();
		
	private:
		struct sockaddr_in	_address;
		int					_socket;
		int					_connection;
		int					_clients[CLI_LIMIT];
		int					_buffers[CLI_LIMIT][BUF_LIMIT];
		int					_cli_pos;
		int					_cli_count;

	// Hidden orthodox canonical shit
		Server& operator=(const Server& other) = default;
};

#endif