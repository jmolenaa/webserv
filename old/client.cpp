/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   client.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: yizhang <yizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 11:49:20 by yizhang       #+#    #+#                 */
/*   Updated: 2024/04/16 13:43:35 by yizhang       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
  
int main() 
{ 
    // creating socket 
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
  
    // specifying address 
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 
  
    // sending connection request 
    connect(clientSocket, (struct sockaddr*)&serverAddress, 
            sizeof(serverAddress)); 
  
    // sending data 
    while(1)
    {
        const char* message = "Hello, server!"; 
        send(clientSocket, message, strlen(message), 0); 
        
         int webSocket = accept(serverSocket, nullptr, nullptr);
        char buffer[1024] = { 0 }; 
        recv(clientSocket, buffer, sizeof(buffer), 0); 
        std::cout << "Message from WEB: " << buffer << std::endl;
    }
  
    // closing socket 
    close(clientSocket); 
  
    return 0; 
}