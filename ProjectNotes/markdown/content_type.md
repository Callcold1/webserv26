# HTTP BODY : CONTENT TYPE

## 1. Text-based formats

- Content-Type: text/html; charset=UTF-8
- Content-Type: text/plain; charset=ISO-8859-1
- Content-Type: text/css
- Content-Type: text/javascript

## 2. Image formats

Content-Type: image/png
Content-Type: image/jpeg
Content-Type: image/gif

## 3. Application-specific formats

Content-Type: application/json
Content-Type: application/xml
Content-Type: application/pdf
Content-Type: application/octet-stream  (for binary files)

## 4. Multipart/form-data (for file uploads in POST requests)

Content-Type: multipart/form-data; boundary=----WebKitFormBoundary

## 5. Form data (for URL-encoded form submissions)

Content-Type: application/x-www-form-urlencoded

---

## Usage in HTTP Requests

- In POST/PUT requests, the Content-Type header tells the server how to interpret the request body.

>POST `/upload` `HTTP/1.1`
>Host: `example.com`
>Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryXYZ
>
>----WebKitFormBoundaryXYZ
>Content-Disposition: form-data; name="file"; filename="example.txt"
>Content-Type: text/plain
>
>(file data)

## Usage in HTTP Responses

- In responses, the Content-Type tells the browser how to handle the response data.

>HTTP/1.1 200 OK
>Content-Type: `text/html`; charset=UTF-8
>Content-Length: 1234
>
>`<html>...</html>`

---

## 1. Chunked Transfer Encoding with Multipart Data

In HTTP/1.1, chunked transfer encoding allows the server to send a response in chunks without specifying the total Content-Length. When combined with multipart/form-data, the data is split into multiple parts, each with its own headers.
Example of Chunked Multipart Data (File Upload)

### A server response with chunked transfer encoding and multipart/form-data might look like this

>HTTP/1.1 200 OK
>Transfer-Encoding: chunked
>Content-Type: multipart/form-data; boundary=----Boundary123
>
>4\r\n
>----Boundary123\r\n
>24\r\n
>Content-Disposition: form-data; name="file"; filename="example.txt"\r\n
>Content-Type: text/plain\r\n
>\r\n
>7\r\n
>Hello, \r\n
>6\r\n
>World!\r\n
>2\r\n
>\r\n\r\n
>4\r\n
>----Boundary123--\r\n
>0\r\n
>\r\n

### Breakdown of the Above Example

    1. The response uses Transfer-Encoding: chunked.
    2. Each chunk is prefixed with its size in hexadecimal.
    3. The multipart/form-data structure remains, but each part is sent in separate chunks.
    4. 0\r\n\r\n at the end indicates the end of the chunked response.

## 2. Chunked Transfer Encoding with Plaintext

If you’re only dealing with text/plain, the response might look simpler:

>HTTP/1.1 200 OK
>Transfer-Encoding: chunked
>Content-Type: text/plain
>
>7\r\n
>Hello, \r\n
>6\r\n
>World!\r\n
>2\r\n
>\r\n\r\n
>0\r\n
>\r\n

Each chunk contains a length header, followed by the actual data.

## 3.Handling in your Web Server

- If your server receives Transfer-Encoding: chunked, it must read chunks sequentially until the 0 chunk.
- For multipart/form-data, the server must parse the boundary-based structure within the chunked stream.
- If the Content-Type is text/plain, simply concatenate the chunks to reconstruct the message.

## Multipart Chunked Transfer Encoding in HTTP/1.1

## 1. Can Multipart Data Be Chunked?

Yes, **multipart data can be sent using chunked transfer encoding** in HTTP/1.1, though it's uncommon. Some web servers, reverse proxies, and APIs may not fully support it.

## 2. Multipart + Chunked Transfer Encoding

### How It Works

- `Transfer-Encoding: chunked` means the body is sent in chunks without a predefined `Content-Length`.
- `Content-Type: multipart/form-data; boundary=...` means the request or response body contains multiple parts, each with headers and separated by a boundary.
- When used together, the boundary-separated parts arrive in chunks.

### Example: Chunked Multipart Request (File Upload)

A **client** might send a `POST` request with chunked `multipart/form-data`:

```
POST /upload HTTP/1.1
Host: example.com
Transfer-Encoding: chunked
Content-Type: multipart/form-data; boundary=----Boundary123

4\r\n
----Boundary123\r\n
1E\r\n
Content-Disposition: form-data; name="file"; filename="example.txt"\r\n
Content-Type: text/plain\r\n
\r\n
7\r\n
Hello, \r\n
6\r\n
World!\r\n
2\r\n
\r\n\r\n
4\r\n
----Boundary123--\r\n
0\r\n
\r\n
```

#### Breakdown

- Chunks are sent one by one.
- Each `multipart` boundary part arrives as part of chunked data.
- The final `0\r\n\r\n` signals the end of the body.

## 3. Handling Chunked Multipart in a Web Server

Your C++98 web server should:

1. **Detect **``** in the request.**
2. **Read and decode chunks**:
   - Extract chunk size.
   - Read the chunked data.
   - Stop at `0\r\n\r\n`.
3. **Parse **``:
   - Identify boundaries.
   - Extract headers (`Content-Disposition`, `Content-Type`).
   - Store file data accordingly.

## 4. Is It Standard?

- HTTP/1.1 **allows** `multipart/form-data` to be sent with `Transfer-Encoding: chunked`, but **not all servers or clients support it**.
- Many implementations expect `Content-Length` in `multipart/form-data` requests.

## 5. Alternative Approach

If your server doesn’t need to support chunked `multipart/form-data`, you can require clients to send `Content-Length` instead.

---
