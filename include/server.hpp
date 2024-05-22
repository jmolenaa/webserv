/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 11:33:39 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <set>
# include <iostream>
# include <unistd.h>
# include <cstring>

# include "defines.hpp"
# include "epoll.hpp"
# include "serverConfig.hpp"

class Server
{
	public:
		Server(Epoll& epoll, ServerConfig& config);
		~Server();

		void	run();

	private:
		Epoll&					_epoll;
		ServerConfig			_config;
		int						_serverfd;

		void	createSocket();
		void	bindToAddress();
		void	setupEpoll();

		void	handleEvents(epoll_event* events, int numEvents);
		void	handleNewConnection();
		void	handleClientRequest(int fd);
		static void	serveClient(int clientFd, const std::string& message);

	// Hidden orthodox canonical shit
	// Server& operator=(const Server& other) = default;
};

#endif