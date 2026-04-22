# BODY SIZE

When dealing with HTTP/1.0 and HTTP/1.1, “body size” isn’t controlled by a single field—it’s determined by a combination of headers and protocol rules. The key is understanding how the receiver knows where the body ends.

Here’s a clean breakdown of the important parameters and what they mean.

(very useful for your validator)

## Priority Order (How to Determine Body Size)

HTTP/1.1:

    if         Transfer-Encoding: chunked  → use chunk parsing
    else if    Content-Length              → read exact bytes
    else if    Connection: close           → read until EOF
    else                                   → no body

## 1. Content-Length (HTTP/1.0 & 1.1)

A header that explicitly tells the size of the message body in bytes.

    Content-Length: 348

### Meaning1

The body is exactly 348 bytes long
The receiver reads exactly that many bytes after headers

### Importance_1

Primary mechanism in HTTP/1.0
Still heavily used in HTTP/1.1

    Required when:
        Sending a body without chunked encoding
        Want persistent connections (keep-alive)

### Notes_1

If incorrect → causes desync (very dangerous for your server)
Must be a non-negative decimal integer

## 2. Transfer-Encoding: chunked (HTTP/1.1 only)

What it is

A way to send the body in chunks, each with its own size.

Transfer-Encoding: chunked

### Meaning_2

    Body is split into chunks:
        4\r\n
        Wiki\r\n
        5\r\n
        pedia\r\n
        0\r\n
        \r\n

    Each chunk:
        Starts with size in hex
        Ends with 0 chunk → end of body
        Importance
        Allows streaming (unknown body size upfront)
    
    Used for:
        Dynamic content
        CGI output
        Large responses

### Rules_2

Overrides Content-Length (must ignore CL if TE is present)
Only valid in HTTP/1.1

## 3. Connection: close (implicit body length)

What it is

No explicit size header—body ends when connection closes.

    Connection: close

### Meaning_3

Read until EOF (socket close)

### Importance_3

Common in HTTP/1.0

Fallback mechanism when:

- No Content-Length
- No Transfer-Encoding

Downsides:

- Cannot reuse connection
- Inefficient

## 4. No Body Cases (implicit zero length)

Some responses must not have a body, regardless of headers.

### Cases

Request methods:
    HEAD

Response status codes:

- 1xx (informational)
- 204 No Content
- 304 Not Modified

### Meaning_4

Body size = 0
Ignore Content-Length if present

## 5. Multipart Boundaries (indirect size control)

What it is

Used in:

    Content-Type: multipart/form-data; boundary=----XYZ

### Meaning_5

- Body is split into parts separated by boundary strings
- Size is not given globally, but structure defines parsing

### Importance_5

Used for:
    File uploads (POST)

You still rely on:

    Content-Length OR
    chunked

## Our Rules

    chunked: Only valid in HTTP/1.1
    mp:
    body other:

## 6. Trailer Headers (HTTP/1.1, chunked only)

What it is

Headers sent after the body.

    Trailer: Content-MD5

### Meaning_6

- Appear after the final chunk
- Do not define size, but affect parsing completeness

## 7. Content-Encoding (NOT size, but affects interpretation)

    Content-Encoding: gzip

### Meaning_7

- Body is compressed
- Content-Length refers to compressed size, not original

## BODY DETECTION

                +---------------------+
                |     READ_HEADERS    |
                +---------------------+
                         |
                         | headers complete
                         v
          +---------------------------------------+
          |        DETERMINE_BODY_MODE            |
          +---------------------------------------+
           |        |               |         |
           |        |               |         |
           v        v               v         v
     [NO_BODY] [CONTENT_LENGTH] [CHUNKED] [UNTIL_CLOSE]
           |        |               |         |
           |        |               |         |
           v        v               v         v
         DONE   READ_BODY  READ_CHUNKS  READ_UNTIL_EOF
                     |         |              |
                     v         v              v
                   DONE      DONE            DONE

Actions
=======

Update variable :  body_bytes_expected (after parsing content-length header)

Add check for chunked in validator : if not HTTP 1.1, error
Add trailer parsing logic and validation (parsing is ok I think, but need to check)

Add logic : - if no TE or content-length, parse body until read == 0
            - allow EOF (i.e. read == 0) to indicate end of body (assuming write end of pipe hasn"t been closed)
            - add at start of parser (if read == 0 and has_length == false)

Check if we can "half-close" connection : should we? Means we can await confirmation that client is closing connection when we send response, but close read side

Once parsing complete or read == 0:
                                               if (body_bytes_expected && body_bytes_read < body_bytes_expected)
                                                   -> ERROR (incomplete message)
                                               if (!received_final_chunk)
                                                   -> ERROR
