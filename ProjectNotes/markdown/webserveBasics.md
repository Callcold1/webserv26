# **SERVER BASICS**

## **Host:Port and Multiple Servers**

In a web server, the concept of `host:port` refers to the combination of a
**network address (host)** and a **communication port** on which the server is **listening**.

The **host** is usually an **IP address** or a **domain name**
    e.g., ```example.com```

and the **port** is a **numeric value** that represents a specific communication channel
    e.g., `port 80 for HTTP`

---

## **Multiple Servers and Configuration**

In the configuration file, multiple servers can be created to listen on different `host:port` combinations.

For example, a server could be configured to listen on
    `127.0.0.1:8080`
    or
    `example.com:80`

Each **server block** can define the following:

### **1. Port and Host:**

You can specify the **port** :
    e.g., `8080`, `443`, etc

and the **host** :
    e.g., `localhost`, ```example.com```

### **2. Server Names:**

Optionally, you can assign specific server names
    e.g., ```www.example.com```
    or
    ```api.example.com```

<div style="page-break-after: always"></div>

## **Default Server Name**

If no server name is provided, the **first server defined** for a specific `host:port` combination will act as the **default** for all requests to that combination.

For example, a **configuration** might look like:

    server {
        host: 127.0.0.1
        port: 8080
        server_name: "localhost"
    }
    server {
        host: example.com
        port: 80
        server_name: "www.example.com"
    }

Here, the server on ```127.0.0.1:8080``` would only respond to **requests** made to that **IP address** and **port**,
while the server on ```example.com:80``` would respond to **requests** made to that **domain** and **port**.

If a **request** is made to `example.com:80` without matching a specific **route** or **configuration**,
it will fall back to the **default configuration** for that `host:port`.

<div style="page-break-after: always"></div>

## **ROUTING CONFIGURATION AND METHODS**

---

### **Routes**

**Routes** define how the server will **respond** to different types of requests.

Each route can specify:

---

**1. Accepted HTTP Methods:**

  These are the allowed HTTP methods for that route
  e.g., `GET`, `POST`, `DELETE`, `PUT`

  For example, if a route only **accepts** `GET`, it will **reject** any `POST` or other requests.
  
  A route could be configured as:

      route {
      path: /index.html
      methods: GET, POST
      }

---

**2. HTTP Redirection:**

  Routes can also define **HTTP redirections**
  When a request matches a **route**, instead of **serving content**, the server can send a **redirect response**
  
  e.g., a `301 Moved Permanently` or `302 Found status`, **redirecting** the client to **another resource**

      route {
      path: /old-page
      methods: GET
      redirect: /new-page
      }

This means that when a `GET` request is made to **/old-page**, the server will respond with a redirection to **/new-page**

<div style="page-break-after: always"></div>

### **Example of Server and Route Configuration**

---

Let’s combine these concepts into a **sample configuration**:

    server {
        host: 127.0.0.1
        port: 8080
        server_name: "localhost"
    
        route {
            path: /index.html
            methods: GET
            handler: serve_index_html
        }
       route {
           path: /submit
           methods: POST
           handler: handle_form_submission
        }
    }

    server {
        host: example.com
        port: 80
        server_name: "www.example.com"
    
        route {
            path: /old-page
            methods: GET
            redirect: /new-page
        }
    }

The first server **listens** on ```127.0.0.1:8080``` and **responds** to `GET` requests at `/index.html`, and `POST` requests at `/submit`.
The second server **listens** on ```example.com:80``` and **redirects**  `GET` requests from `/old-page` to `/new-page`.

In this setup:

Requests to ```127.0.0.1:8080/index.html``` will trigger the **serve_index_html handler**
Requests to ```example.com/old-page``` will result in a **redirect** to ```example.com/new-page```.

<div style="page-break-after: always"></div>

## **KEY CONSIDERATIONS**

---

**Host:Port Matching:**

- The host:port defines which server block handles the request.
- If a client connects to ```example.com:80``` and no specific route matches, the server configuration with ```example.com:80``` will handle the request.
- If no match exists in that block, the default behavior is invoked.

---

### **Routing Without Regular Expressions**

- Routes are defined using paths that match exactly.
- For example, ```/index.html``` will match only that specific path and not any other variation e.g. ```/index.html?query=123```
- Routes do not rely on regular expressions to match URLs

---

### **Default Server**

- If a ```host:port``` combination has **no explicit server name** defined, the **first server block** matching that combination will **handle** all requests for that ```host:port```
- This configuration setup allows for flexibility in handling multiple servers, each with its own set of routes and behaviors, without the need for regular expressions to match paths.

<div style="page-break-after: always"></div>

## **poll(), FILE DESCRIPTORS (fd), SOCKETS, and CLIENTS**

---

### **1. Sockets**

**Sockets** are **endpoints** for communication between two systems over a network. They are used to **send** and **receive** data between the **server** and **clients**.

In a web server context, the server creates a **listening socket** that waits for **incoming connection requests** from clients. When a client makes a request,
the server **accepts** the connection, which results in the creation of a **new socket** for communication with that specific client.

---

### **2. File Descriptors (fd)**

**File descriptors** are **integer values** that represent **open files** or **sockets**. In Unix-like systems, **sockets** are treated as files,
and each socket has an associated file descriptor.

When the server calls `socket()`, it gets a **file descriptor** associated with the socket.

When a client connects to the server, the server **accepts** the connection and gets a new **file descriptor** for that particular client socket,
which is used to **read from** or **write to** the client.

---

### **3. poll()**

`poll()` is used for **multiplexing** I/O operations. In **non-blocking mode**, it monitors multiple file descriptors (including sockets)
to check if they are ready for reading, writing, or have errors.

`poll()` allows the server to handle multiple connections with a single call by checking whether any socket has data to be read or is ready for writing.
It avoids the need to **block** the process while waiting for client requests or responses.

---

### **4. Clients**

Clients are the remote entities (e.g., browsers) that establish a connection to the server and send requests. Each client gets its own **socket** (a file descriptor).

The server uses `poll()` to monitor the sockets of multiple clients, so it can handle requests from many clients **concurrently** without blocking on any individual connection.

<div style="page-break-after: always"></div>

## **HOW IT FITS TOGETHER**

---

### **1. Server Setup**

The server creates a **listening socket** and binds it to a port.
The listening socket waits for incoming client connection requests.

### **2. Client Connects**

When a client connects, the server **accepts** the connection, creating a new **client socket** with its own **file descriptor (fd)**.

### **3. Using `poll()`**

The server calls `poll()` with an array of file descriptors that includes the listening socket and any open client sockets.
`poll()` checks the file descriptors to see if any are **ready to read**, meaning a client has sent a request.
If the listening socket is ready, the server accepts the incoming connection and adds the new client`s socket to the list of file descriptors.
If any client sockets are ready to read, the server reads the data (request) from those sockets.

### **4. Handling Multiple Clients**

By using `poll()`, the server can handle multiple clients in a non-blocking manner. It can process multiple connections without waiting for one client to finish before handling another.
When `poll()` detects that a client socket is ready to write, the server can send the response to that client.
The server can loop through this process, efficiently handling requests and responses for many clients, all within the same event loop.

<div style="page-break-after: always"></div>

## **EXAMPLE FLOW**

---

### 1. The server sets up the listening socket

### 2. `poll()` is called, monitoring both the listening socket and any client sockets

### 3. The server waits for activity on any of the monitored sockets (client requests)

### 4. When a client connects, a new client socket is added to the poll list

### 5. `poll()` detects that the listening socket is ready, so the server accepts the new connection and adds the new client socket to the poll list

### 6. `poll()` detects a client socket is ready for reading, the server reads the request, processes it, and sends a response back

### 7. This continues for all clients, with `poll()` acting as the core mechanism for checking multiple sockets
