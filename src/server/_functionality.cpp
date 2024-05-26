/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _functionality.cpp                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:16:25 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/24 13:33:53 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"
#include "response.hpp"
#include "log.hpp"

/**
 * Accepts new connection on socket and adds to epoll instance
*/
void Server::_handleNewConnection() 
{
	sockaddr_in clientAddr{};
	socklen_t clientAddrLen = sizeof(clientAddr);
	int clientFd = accept(_serverfd, reinterpret_cast<sockaddr *>(&clientAddr), &clientAddrLen);
	if (clientFd == -1)
		throw WebservException("Failed to handle a new connection because: " + std::string(std::strerror(errno)) + "\n");
	_epoll.addFd(clientFd, EPOLLIN);
}

void Server::_handleClientRequest(int fd)
{
	char buffer[1024];
	ssize_t numBytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (numBytes <= 0)
		close(fd);
	else
	{
		buffer[numBytes] = '\0';
		Request request(buffer);

		std::string path = request.getPath();
		Location loc = _config.matchLocation(path);
		while (!path.empty() && path != loc.path) //double check this shit
		{
			// std::cout << path << std::endl; //DEEBUGGING
			size_t end = path.find_last_of('/');
			if (end == std::string::npos)
				break;
			else
				path = path.substr(0, end);
			loc = _config.matchLocation(path);
		}
		Response response(_epoll, request, loc);
		_serveClient(fd, response.getResponseMessage());
	}
}

void Server::_serveClient(int clientFd, const std::string& message)
{
	Log::getInstance().print("\n=====SEND=====\n" + message + "\n=====END=====\n");
	
	ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
	close(clientFd);
	if (bytesSent == -1)
		throw (WebservException("Failed to send response: " + message + "\n"));
}
