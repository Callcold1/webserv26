# Web Server Functions in C++98

## 1. Initialization and Configuration
- **`parseConfigFile(const std::string& filename)`** - Reads and parses the configuration file to set up server parameters (ports, routes, etc.).
- **`setupSockets()`** - Creates, binds, and listens on the necessary sockets based on the configuration.
- **`setNonBlocking(int fd)`** - Configures a socket or file descriptor as non-blocking.

## 2. Event Loop (Non-blocking Polling)
- **`runEventLoop()`** - Main server loop that monitors events on all sockets using `poll()`, `epoll()`, or `kqueue()`.
- **`acceptNewConnection(int server_fd)`** - Accepts incoming client connections and adds them to the event loop.
- **`handleClientEvents(int client_fd, short events)`** - Processes events (read/write/close) for a given client.

## 3. HTTP Request Handling
- **`readRequest(int client_fd)`** - Reads and buffers incoming HTTP request data.
- **`parseHTTPRequest(int client_fd, std::string& buffer)`** - Parses buffered request data and extracts method, headers, and body.
- **`validateHTTPRequest(const HTTPRequest& request)`** - Checks for malformed requests, missing headers, and compliance with HTTP/1.1.

## 4. HTTP Response Handling
- **`generateResponse(const HTTPRequest& request)`** - Determines the appropriate response based on the request method (GET, POST, DELETE).
- **`sendResponse(int client_fd, const HTTPResponse& response)`** - Sends the generated HTTP response back to the client.
- **`handleChunkedTransferEncoding(int client_fd, std::string& buffer)`** - Implements chunked transfer decoding when required.
- **`sendStaticFile(int client_fd, const std::string& filepath)`** - Reads and streams static files (e.g., HTML, CSS, JS) as responses.

## 5. Connection and Timeout Handling
- **`closeClientConnection(int client_fd)`** - Closes client connections when necessary.
- **`checkTimeouts()`** - Identifies and closes idle connections to prevent resource exhaustion.

## 6. CGI Handling (For Dynamic Content)
- **`handleCGIRequest(const HTTPRequest& request)`** - Forks a child process to execute CGI scripts (if applicable).
- **`setupCGIPipes(int& read_fd, int& write_fd)`** - Sets up pipes for CGI input/output handling.

## 7. Logging and Debugging
- **`logRequest(const HTTPRequest& request, int client_fd)`** - Logs HTTP requests for debugging.
- **`logError(const std::string& errorMessage)`** - Logs server errors and issues.

## 8. Graceful Shutdown
- **`shutdownServer()`** - Closes all open sockets, cleans up resources, and exits safely.

