/* ************************************************************************** */
/*																			*/
/*														::::::::			*/
/*   _client_handlers.cpp							   :+:	:+:			*/
/*													 +:+					*/
/*   By: dliu <dliu@student.codam.nl>				 +#+					 */
/*												   +#+					  */
/*   Created: 2024/04/19 17:06:10 by dliu		  #+#	#+#				 */
/*   Updated: 2024/04/19 17:06:34 by dliu		  ########   odam.nl		 */
/*																			*/
/* ************************************************************************** */

#include "server.hpp"
#include "request.hpp"

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
		// send back files, implement CGI.
		serveClient(fd, "You've been served\n");
	}
}

void Server::serveClient(int clientFd, const std::string& message)
{
	ssize_t bytesSent = send(clientFd, message.c_str(), message.size(), 0);
	if (bytesSent == -1)
		std::cerr << "Failed to send message: " << std::strerror(errno) << std::endl;
}
