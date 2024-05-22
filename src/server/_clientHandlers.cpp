/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   _clientHandlers.cpp                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: dliu <dliu@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/17 14:16:12 by dliu          #+#    #+#                 */
/*   Updated: 2024/05/22 14:39:04 by dliu          ########   odam.nl         */
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
		Request request(buffer);
		request.printData(); //REMOVE this if you don't want to print the request

		std::string path = request.getPath();
		Location loc = _config.matchLocation("root");
		while (!path.empty() && path != loc.path)
		{
			loc = _config.matchLocation(path);
			size_t end = path.find_last_of('/');
			if (end == std::string::npos)
				path = "";
			else
				path = path.substr(0, end);
		}

		Response response(_epoll, request, loc);
		serveClient(fd, response.getResponseMessage());
	}
}

/**
 * @todo make this a try catch that throws
*/
void Server::serveClient(int clientFd, const std::string& message)
{
	//REMOVE this if you don't want to print the response
	std::cout << "\n---------SENDING MESSAGE---------\n"
		<< message
		<< "\n---------END---------"
		<< std::endl;

	ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
	if (bytesSent == -1)
		std::cerr << "Failed to send message: " << std::strerror(errno) << std::endl;
	close(clientFd);
}
