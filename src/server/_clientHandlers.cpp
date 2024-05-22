/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _clientHandlers.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:16:12 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 11:36:15 by dliu          ########   odam.nl         */
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
		close(fd);
	else
	{
		buffer[numBytes] = '\0';
		std::cout << "----Received request----\n" << buffer << "\n--------\n" << std::endl;

		Request request(buffer);
		request.printData(); //REMOVE this if you don't want to print the request

		Response response(request, this->_config); //will need to update to handle PUT and DELETE
		serveClient(fd, response.getResponseMessage());
	}
}

void Server::serveClient(int clientFd, const std::string& message)
{
	std::cout << "\n------------SENDING MESSAGE----------\n" << message << "\n------------\n" << std::endl;
	ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
	if (bytesSent == -1)
		std::cerr << "Failed to send message: " << std::strerror(errno) << std::endl;
	close(clientFd);
}
