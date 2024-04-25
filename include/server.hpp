/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/25 17:13:26 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include <set>
# include "webserv.hpp"

class Server
{
	public:
		public:
		Server();
		~Server();
		
		void	run();

	private:
		int				_serverfd;
		int				_epollfd;
		std::set<int>	_clientfds;

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