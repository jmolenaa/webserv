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

/**
 * Retrieves message from client and serves requests
 * 
 * @todo
 * read from the socket until you encounter the 0x0D 0x0A sequence of bytes denoting the end of the response line, which contains the HTTP version, Status Code, and Reason text.
 * then read from the socket until you encounter the 0x0D 0x0A 0x0D 0x0A sequence of bytes denoting the end of the response headers.
 * then analyze the response line and headers to know if a message body is present, and if so in what format it is being sent as, which dictates how you must read it.
 * read the message body, if present:
 * if the response Status code is 1xx, 204, or 304, or if the response is to a HEAD request, no message body is present.
 * otherwise, if a Transfer-Encoding header is present and has a value other than identity, read the message body in chunks until a 0-length chunk is read.
 * otherwise, if a Content-Length header is present, read from the socket until the exact number of bytes specified have been read, no more, no less.
 * otherwise, if the Content-Type header indicates a multipart/... media type, read from the socket and parse the MIME data until the final terminating MIME boundary is reached.
 * otherwise, read from the socket until the connection is closed.
 * if the response is not read in full successfully, or if a keep-alive is NOT being used (a Connection: close header is present in an HTTP 1.1 response, or a Connection: keep-alive header is not present in an HTTP 1.0 response), then close the socket.
 * This is covered by RFC 2616 (Section 4.4 and Section 8), and by RFC 7230 (Section 3.3.3 and Section 6).
*/
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
		std::cout << "Received: " << buffer << std::endl;
		// Add code to parse the client request, 
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
