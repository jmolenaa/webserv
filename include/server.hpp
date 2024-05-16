/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/16 13:05:12 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/epoll.h>
# include "webserv.hpp"
# include "location.hpp"
# include "epoll.hpp"
# include <vector>


class Server
{
	public:
		Server();
		~Server();
		
		void	run();

	private:
		int						_serverfd;
		Epoll					_epoll;
		std::set<int>			_clientfds;

		std::string				_name;
		std::vector<Location>	_locations; //default location first, followed by others

		void	createSocket();
		void	bindToAddress();
		void	setupEpoll();

		void	handleEvents(epoll_event *events, int numEvents);
		void	handleNewConnection();
		void	handleClientRequest(int fd);
		static void	serveClient(int clientFd, const std::string& message);

	// Hidden orthodox canonical shit
		Server& operator=(const Server& other) = default;
};

#endif

