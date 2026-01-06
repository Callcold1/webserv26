# Headers

## Types of header include

`Request headers`
Contain more information about the resource to be fetched, or about the client requesting the resource.

`Response headers`
Hold additional information about the response, like its location or about the server providing it.

`Representation headers`
Contain information about the body of the resource, like its MIME type, or encoding/compression applied.

`Payload headers`
Contain representation-independent information about payload data, including content length and the encoding used for transport.

## Headers can also be **grouped** according to how proxies handle them

`End-to-end headers`
These headers must be transmitted to the final recipient of the message: the server for a request, or the client for a response. Intermediate proxies must retransmit these headers unmodified and caches must store them.

`Hop-by-hop headers`
These headers are meaningful only for a single transport-level connection, and must not be retransmitted by proxies or cached. Note that only hop-by-hop headers may be set using the Connection header.

## 1. Headers That Make More Sense for Certain Methods

While most headers can technically be used with any method, some are only meaningful in specific contexts:

    GET Requests:
        Headers like Content-Length and Content-Type are usually not used, since GET requests typically don't have a body.
        Range is commonly used for requesting partial content.
        Cache-Control and If-Modified-Since are useful for cache validation.

    POST and PUT Requests:
        These methods usually include Content-Length or Transfer-Encoding: chunked to indicate request body size.
        Content-Type is crucial to specify the media type of the request body (e.g., application/json, multipart/form-data).
        Expect: 100-continue can be used to check if the server is willing to accept a large payload before sending it.

    DELETE Requests:
        Usually, DELETE requests don't require a body, but HTTP/1.1 allows it.
        Some APIs may use headers like If-Match or If-Unmodified-Since to prevent accidental deletions.

    OPTIONS Requests:
        Used for CORS and feature discovery, often including Access-Control-Request-Method and Access-Control-Request-Headers.

    HEAD Requests:
        Should not contain a body, and the response must not include one, even if Content-Length is specified.

## 2. Headers That Are Restricted in Certain Situations

    Some headers like Host are mandatory in HTTP/1.1, regardless of the method.
    Transfer-Encoding: chunked is allowed in requests for methods that include a body (like POST, PUT) but not in a response to a HEAD request.
    Security-related headers (Referer, Origin, Authorization) may be restricted or ignored based on the context (e.g., CORS restrictions in browsers).
    Some headers (Content-Length, Transfer-Encoding) are mutually exclusive in request bodies.

## 3. Method-Specific Header Behavior

    Some headers change meaning depending on the method:
        Location in a response is typically used with 302 Found (redirect), but in a PUT response, it may indicate the newly created resource.
        Allow is typically sent in responses to OPTIONS or 405 Method Not Allowed.

So, while HTTP itself does not strictly prevent headers based on the method, practical usage and standards define how they should behave in context. Are you dealing with a specific case in your web server?

## Terminal Control Characters (Saw this in ABNF document)

Are you asking about terminal (control) characters in HTTP headers or something related to terminating characters in HTTP messages? Here are a few possibilities based on your question:

### 1. HTTP Header Line Termination (CRLF)

-Each HTTP header line is terminated with \r\n (CRLF).
>Example:
>
>Host: example.com\r\n
>User-Agent: CustomClient\r\n
-The entire header section is terminated by an extra \r\n (CRLF), marking the end of headers.

### 2. Forbidden Characters in HTTP Headers

HTTP headers must not contain unescaped control characters (ASCII values 0x00-0x1F, except for \r and \n in their specific roles).
Forbidden characters include NULL (\x00), BEL (\x07), ESC (\x1B), etc.
Spaces and tabs (SP, HTAB) are allowed for folding long headers (deprecated in HTTP/1.1).

### 3. Terminal Characters in HTTP Message Body

For chunked transfer encoding, each chunk is terminated by \r\n, and the entire message ends with a zero-length chunk (0\r\n\r\n).
Content-Length-based bodies don't need special terminators; they simply end after the declared number of bytes.
