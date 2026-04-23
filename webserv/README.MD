# *This project has been created as part of the 42 curriculum by <login1>[, <login2>]*

## Description

**Webserv** is a custom HTTP server implemented in C++, built from scratch to understand the fundamentals of web communication and server architecture.

The goal of this project is to reproduce the behavior of a real-world web server (such as Nginx) while respecting the HTTP/1.1 protocol. It handles client connections, parses HTTP requests, generates appropriate responses, and serves static and dynamic content.

Key concepts explored in this project include:
- Socket programming and network communication
- Event-driven architecture using multiplexing (e.g., `select`, `poll`, or `epoll`)
- HTTP request parsing and response generation
- Configuration file parsing (similar to Nginx)
- Handling multiple clients concurrently
- CGI (Common Gateway Interface) execution

---

## Features

- HTTP/1.1 compliant request handling
- Support for multiple server blocks and ports
- Configurable routing (location blocks)
- Static file serving
- File uploads
- CGI execution (e.g., PHP, Python scripts)
- Custom error pages
- Non-blocking I/O and client multiplexing

---

## Instructions

### Requirements

- C++ compiler (C++98 standard)
- Unix-based system (Linux or macOS)
- `make`

### Compilation

```bash
make