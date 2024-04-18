/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: yizhang <yizhang@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/04/16 11:48:53 by yizhang       #+#    #+#                 */
/*   Updated: 2024/04/17 16:42:05 by dliu          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


# include <iostream>
# include <sys/socket.h> //socket
# include <unistd.h> //close
# include <netinet/in.h> //sockaddr_in
#include <cstring>

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        std::cout<<"get config:"<<argv[1]<<std::endl;
        
        //creating socket
        int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        std::cout<<serverSocket<<std::endl;
        //serverSocket: It is the file descriptor for the socket.
        //AF_INET: It specifies the IPv4 protocol family.
        //SOCK_STREAM: It defines that the TCP type socket.
    
        //specifying the address, defining server address
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET; 
        serverAddress.sin_port = htons(8080); 
        serverAddress.sin_addr.s_addr = INADDR_ANY; 
        
        //binding socket
        bind(serverSocket, (struct sockaddr*)&serverAddress, 
         sizeof(serverAddress)); 
         
        //listening to the assigned socket
            listen(serverSocket, 5); 
        
            while (1) {
            //accepting connection request
            // int clientSocket = accept(serverSocket, (struct sockaddr *)&serverAddress, (socklen_t*)sizeof(serverAddress));
            int clientSocket = accept(serverSocket, nullptr, nullptr);
            std::cout << "huh\n";
            //recieving data
            char buffer[1024] = { 0 }; 
            recv(clientSocket, buffer, sizeof(buffer), 0); 
            std::cout << "Message from client: " << buffer << std::endl;
            char input[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
            // input[999] = '\0';
            // std::cin  >> input;
            // input = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
            send(clientSocket, input, std::strlen(input), 0); 
            //close the socket
            close(clientSocket);    
        }
        //close(serverSocket);
    }
    else
    {
        std::cout<<"Invalid number of arguments."<<std::endl;
        return 1;
    }
    return 0;
}