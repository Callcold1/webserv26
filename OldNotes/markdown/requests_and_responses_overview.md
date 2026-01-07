
# HTTP REQUEST AND RESPONSE STRUCTURES

## HTTP Request Structure

An HTTP request typically consists of a `request line`, a `header` and an optional `body`:

---

### **Request Line**

>This contains:
>
>- The `method`
>- The `requested resource` (URL)
>- The `HTTP version`
>
>Example:
>`GET /index.html HTTP/1.1`

---

### **Headers**

>These provide metadata about the request (e.g., content type, user-agent, etc.).
>
>Example:
>`Host: example.com`
>or
>`Content-Type: application/json`

---

### **Body** _(optional, depending on the method)_

>It contains data sent with the request, such as:
>
>- Form data
>- File upload
>
>Example (for POST):
>`name=John&age=30`

<div style="page-break-after: always"></div>

#### GET Request Example

>`GET /index.html HTTP/1.1`
>`Host: example.com`
>`Connection: keep-alive`

#### POST Request Example

>`POST /submit HTTP/1.1`
>`Host: example.com`
>`Content-Type: application/x-www-form-urlencoded`
>`Content-Length: 19`
>
>`name=John&age=30`

#### DELETE Request Example

>`DELETE /index.html HTTP/1.1`
>`Host: example.com`

<div style="page-break-after: always"></div>

## HTTP Response Structure

---

An HTTP response typically consists of:

### Status Line

>This includes:
>
>- The HTTP version
>- A status code
>- A reason phrase
>
>Example:
>`HTTP/1.1 200 OK`

### Headers

>These provide metadata about the response, such as:
>
>- Content type
>- Content length
>
>Example:
>`Content-Type: text/html`

### Body

>This contains data sent as part of the response, such as:
>
>- An HTML page
>- File content
>
>Example:
>```<html><body>Success</body></html>```

<div style="page-break-after: always"></div>

#### Example Response for a GET Request

>`HTTP/1.1 200 OK`
>`Content-Type: text/html`
>`Content-Length: 45`
>
>```<html><body>Welcome to my website</body></html>```

#### Example Response for a POST Request

>`HTTP/1.1 201 Created`
>`Content-Type: application/json`
>`Content-Length: 25`
>
>`{"status": "success", "id": 123}`

#### Example Response for a DELETE Request

>`HTTP/1.1 200 OK`
>`Content-Type: text/plain`
>`Content-Length: 10`
>
>`Deleted successfully`

<div style="page-break-after: always"></div>

# HTTP/1.1 Compliance Considerations

>**Host Header**: HTTP/1.1 requires the `Host` header for all requests. It specifies the domain of the server.
>**Connection Header**: Control whether the connection will be kept open (`Connection: keep-alive`) or closed after the request.
>**Content-Length**: If the request or response includes a body, `Content-Length` must specify the size of the body.
>**Chunked Transfer Encoding**: This is used when the size of the response body is unknown or dynamically generated. The body is sent in chunks.
>**Keep-Alive**: HTTP/1.1 defaults to keeping connections open between requests unless explicitly closed (`Connection: close`).
>**Method-Specific Syntax:**
>- `GET`: Requesting data.
>- `POST`: Sending data to the server (e.g., form data).
>- `DELETE`: Deleting a resource.

---

## Parsing with `poll()` in C++98

>Use `poll()` for non-blocking I/O and handle multiple sockets with a single call. Each socket connection (client) will be handled as an individual file descriptor.
>For request parsing:
>- Read the data incrementally as it arrives.
>- First, parse the request line (method, URL, version).
>- Then, parse headers line-by-line.
>- If there`s a body (POST/PUT), parse it after reading the headers.

---

## Consider the following flow

>1. **Initial Poll**: Use `poll()` to check for readability on the sockets.
>2. **Request Parsing**: Once data is available, read and parse the request (method, headers, body).
>3. **Processing**: Process the request and determine which action to take (e.g., fetching a file, handling form data).
>4. **Send Response**: Once the response is ready, write the headers and body back to the client.
>5. **Handle Keep-Alive/Close**: Check if the connection should stay open (`Connection: keep-alive`) or be closed after the request.

---

## Key Considerations for Building a Web Server

>**Concurrency**: Handle multiple connections using non-blocking I/O and `poll()`, ensuring your server can handle many clients at once.
>**Error Handling**: Be sure to respond with appropriate HTTP status codes (e.g., `400 Bad Request`, `404 Not Found`, `500 Internal Server Error`).
>**Security**: Implement input validation to protect against various attacks (e.g., SQL injection, path traversal).
>**Efficiency**: Ensure efficient memory management, especially when parsing request bodies (for methods like POST).
>**Logging**: Consider logging requests and responses for debugging and monitoring.
>**Configuration**: Implement a configuration file for setting server parameters (e.g., port number, document root).
