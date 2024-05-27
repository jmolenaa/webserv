/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/27 12:57:50 by dliu          ########   odam.nl         */
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
		explicit Server(Epoll& epoll, ServerConfigs configs);
		Server() = delete;
		~Server();

		void	run();

	private:
		Epoll&			_epoll;
		int				_serverfd;
		ServerConfigs	_configs;

		void	_createSocket();
		void	_bindToAddress();

		void	_handleEvents(epoll_event* events, int numEvents);
		void	_handleNewConnection();
		void	_handleClientRequest(int fd);
		static void	_serveClient(int clientFd, const std::string& message);

};

#endif