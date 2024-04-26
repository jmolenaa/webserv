/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _clientHandlers.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:16:12 by dliu          #+#    #+#                 */
/*   Updated: 2024/04/26 13:07:00 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"
#include "response.hpp"

void Server::handleClientRequest(int fd)
{
	char buffer[1024];
	ssize_t numBytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (numBytes <= 0)
	{
		_clientfds.erase(fd);
		close(fd);
	} 
	else
	{
		buffer[numBytes] = '\0';
		std::cout << "----Received request----\n" << buffer << std::endl;

		Request request(buffer);
		request.printData();
		Response response(request.getPath());

		//implement CGI.
		serveClient(fd, response.getResponse());
	}
}

void Server::serveClient(int clientFd, const std::string& message)
{
	// write(clientFd, message.c_str(), message.size());
	ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
	if (bytesSent == -1)
		std::cerr << "Failed to send message: " << std::strerror(errno) << std::endl;
	close(clientFd);
}
