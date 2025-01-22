# Webserv

The **Webserv** project focuses on creating an HTTP server from scratch. This project helps you understand the inner workings of HTTP, a protocol foundational to the World Wide Web. By completing this project, you'll gain insights into how servers communicate with clients and deliver resources like web pages. You’ll also test your implementation using a real web browser.
The webserver we build is heavily based on nginx and it's funcitonality. Most of the configuration follows the syntax of nginx.


## Learning Outcomes

Through this project, you will:
- Learn about the HTTP protocol and the workings of webservers and a bit about clients.
- Learn about non-blocking I/o operations and how to implement them.
- Improve on your C++ skills.
- Get more familiar with OOP, by designing your own classes and how they interact with one another.

## General Project Requirements
1. **Error Resilience**: The server must not crash or quit unexpectedly.
2. **Implementation Standards**: Use C++ features and avoid external libraries.
3. **Compatibility**: The server should handle HTTP 1.1 requests and responses accurately.

## Mandatory Features
- **Non-blocking Server**: Uses `epoll()` for all I/O operations, ensuring the server remains responsive.
- **Core HTTP Methods**: Supports `GET`, `POST`, and `DELETE`.
- **Static Website Support**: Serves static files and handle directory browsing if enabled.
- **Uploads**: Allows clients to upload files to specified directories.
- **Error Handling**: Provide default error pages for HTTP errors.
- **Configuration File**: The configuration file enables:
  - Defining ports, hosts, and server names.
  - Setting up routes for specific directories or files.
  - Enabling or disabling features like directory listing and CGI execution.
  - Limiting client body size and defining HTTP redirections.

## Implementation
We used `epoll` for handling the non-blocking part of our I/O operations. <br>
Our classes are based on a restaurant analogy, since we theorized that it might make everything a bit more intuitive, this worked only partially 😅 <br>

1. **Class setup**:
  - The `Restaurant` class is runs the webserver, keeps track of all the file descriptors being handled and contains the infinite loop for the server. It also takes care of kicking out lingering or annoying customers (clients that are in the restaurant for too long, f.e. there is an infinite loop in CGI).
  - The `Concierge` class holds our epoll instance, and handles all epoll operations.
  - The `Waiter` class holds the socket file descriptor of a particular port and host. It also has a map of any `Cook`'s that are identified by this port and host (multiple cooks can exist with the same port and host, but different server_names).
  - The `Cook` class is a server instance, identified by a port, host and a unique server_name. It holds a mapping of `Recipes` or routes with unique configurations (f.e. there might be a /cgi/bin location, an /upload location with different settings from the default).
  - The `Recipe` class holds information on the settings for a current location. It is identified right after parsing the incoming request and then used for figuring out what the response of the server should be (serving a file, executing CGI, redirecting, etc.).
  - The `Customer` class is the client connecting to the webserver. It contains all the information the `Order` (HTTP request) and `Dish` (HTTP response), information about the status of the request and what should happen next. It also handles reading and writing to the client socket.
  - The `Order` class is the HTTP request, holds the headers, body and status of the request (are we still reading from the socket or do we move to responding etc.).
  - The `Dish` class is the HTTP response, it does the operations needed for creating the response, be it reading the file requested, redirecting or running the CGI.
  - The `CGI` class (we ran out of ideas or got tired of the Restaurant analogy), holds all the information about the CGI process, and executes it.

2. **Class relationships**:
In this project a concept that keeps returning is reading and writing to file descriptors. Also, following the rules of the projects, eveyr read and write needs to go through `epoll` first and we also read in chunks of a couple kB at a time (so we're not stuck reading a 5GB file). <br>
Since multiple classes hold file descriptors and the operations they do are very different, instead of identifying each time which file descriptor we have from a vector of Servers, Clients, CGIs or normal files, instead we have one generic class called `FdHandler` from which all classes handling a file descriptor inherit. 

## Key Takeaway
This project is an opportunity to dive deep into HTTP and server mechanics. Even if you're not working directly on web development in the future, understanding HTTP's principles and practical applications will prove invaluable.
