# Webserv

The **Webserv** project focuses on creating an HTTP server from scratch. This project helps you understand the inner workings of HTTP, a protocol foundational to the World Wide Web. By completing this project, you'll gain insights into how servers communicate with clients and deliver resources like web pages. You’ll also test your implementation using a real web browser.
The webserver we build is heavily based on nginx and it's funcitonality. Most of the configuration follows the syntax of nginx.

## Key Features of HTTP
- HTTP enables the transfer of hypermedia documents (e.g., HTML).
- It supports both retrieving and submitting content (e.g., via web forms).
- The protocol is client-server-based, allowing clients like web browsers to request resources from servers.

## General Project Requirements
1. **Error Resilience**: The server must not crash or quit unexpectedly.
2. **Implementation Standards**: Use C++ features and avoid external libraries.
43 **Compatibility**: The server should handle HTTP 1.1 requests and responses accurately.

## Mandatory Features
- **Non-blocking Server**: Uses `epoll()` for all I/O operations, ensuring the server remains responsive.
- **Core HTTP Methods**: Supports `GET`, `POST`, and `DELETE`.
- **Static Website Support**: Serves static files and handle directory browsing if enabled.
- **Uploads**: Allows clients to upload files to specified directories.
- **Configuration File**: The server's behavior, such as port, routes, and error pages can be configured with a configuration file.
- **Error Handling**: Provide default error pages for HTTP errors.

## Configuration File
The configuration file enables:
- Defining ports, hosts, and server names.
- Setting up routes for specific directories or files.
- Enabling or disabling features like directory listing and CGI execution.
- Limiting client body size and defining HTTP redirections.


## Key Takeaway
This project is an opportunity to dive deep into HTTP and server mechanics. Even if you're not working directly on web development in the future, understanding HTTP's principles and practical applications will prove invaluable.
