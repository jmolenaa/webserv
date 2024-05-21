/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/07 15:14:53 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "defines.hpp"
# include "epoll.hpp"
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <set>
# include "location.hpp"
# include <iostream>
# include <unistd.h>
# include <cstring>

class Server
{
	public:
		Server();
		~Server();

		void	run();

	private:
		int						_epollfd;
		int						_serverfd;
		Epoll					_epoll;
		std::set<int>			_clientfds;

		uint16_t				_port;
		uint32_t				_address;
		std::string				_name;
		std::vector<Location>	_locations; //default location first, followed by others

		void	createSocket();
		void	bindToAddress();
		void	setupEpoll();

		void	handleEvents(epoll_event* events, int numEvents);
		void	handleNewConnection();
		void	handleClientRequest(int fd);
		static void	serveClient(int clientFd, const std::string& message);

	// Hidden orthodox canonical shit
		Server& operator=(const Server& other) = default;
};

#endif