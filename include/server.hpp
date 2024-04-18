/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 13:07:45 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/18 17:44:03 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include "webserv.hpp"
# include "epoll.hpp"

class Server
{
	public:
		public:
		Server();
		~Server();

		void	run();
		void	serveClient(int clientFd, const std::string& message);
		void	receiveClient();

	private:
		int		_serverfd;
		Epoll	_epoll;

	// Hidden orthodox canonical shit
		Server& operator=(const Server& other) = default;
};

#endif