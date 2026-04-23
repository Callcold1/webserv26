*This project has been created as part of the 42 curriculum by <junruh>[,<kmooney>].*

# Webserv

## Description
Webserv is a custom HTTP/1.1 server developed in C++ 98. The project focuses on implementing a non-blocking event loop to handle multiple concurrent client connections without the use of threads. It features a robust request parser, a response generator capable of handling different HTTP methods (GET, POST, DELETE), and a configuration system that allows for multiple server blocks, custom error pages, and client body size limits, and more.

## Instructions
To compile and execute the project, follow these steps:

1. Compile the server using the provided Makefile:
make

2. Start the server by passing a configuration file as an argument:
./webserv [path_to_config_file.conf]
or ./webserv without any arguments to go to default configuration

3. To stop the server, use Ctrl+C in the terminal where it is running.

## Resources
- RFC 1945: Official documentation for HTTP/1.1 protocols.
- RFC 9110: Official documentation for HTTP/1.1 protocols.
- RFC 9111: Official documentation for HTTP/1.1 protocols.
- RFC 9112: Official documentation for HTTP/1.1 protocols.
- RFC 3875: Official documentation for CGI protocols.
- https://developer.mozilla.org/en-US/docs/Web/HTTP
- Beej's Guide to Network Programming: Resource for socket-based communication.
- IBM Documentation on poll() and select(): Technical references for I/O multiplexing.
- Linux and C Manuals

AI Usage:
- AI was used to explain certain concepts and technicalities.
- AI helped in generating a variety of HTTP requests for stress testing the server's robustness.
- AI was used to assist in the initial drafting and formatting of this README file.