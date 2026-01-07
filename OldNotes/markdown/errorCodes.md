# HTTP Response Status Codes

All HTTP response status codes are separated into five classes or categories. The first digit of the status code defines the class of response, while the last two digits do not have any classifying or categorization role. There are five classes defined by the standard:

- **1xx** informational response – the request was received, continuing process
- **2xx** successful – the request was successfully received, understood, and accepted
- **3xx** redirection – further action needs to be taken in order to complete the request
- **4xx** client error – the request contains bad syntax or cannot be fulfilled
- **5xx** server error – the server failed to fulfill an apparently valid request

## 1xx Informational Response

An informational response indicates that the request was received and understood. It is issued on a provisional basis while request processing continues. It alerts the client to wait for a final response. The message consists only of the status line and optional header fields, and is terminated by an empty line. As the HTTP/1.0 standard did not define any 1xx status codes, servers must not send a 1xx response to an HTTP/1.0 compliant client except under experimental conditions.


### 100 Continue**  
  The server has received the request headers and the client should proceed to send the request body (in the case of a request for which a body needs to be sent; for example, a POST request). Sending a large request body to a server after a request has been rejected for inappropriate headers would be inefficient. To have a server check the request's headers, a client must send `Expect: 100-continue` as a header in its initial request and receive a 100 Continue status code in response before sending the body. If the client receives an error code such as 403 (Forbidden) or 405 (Method Not Allowed), then it should not send the request's body. The response `417 Expectation Failed` indicates that the request should be repeated without the Expect header as it indicates that the server does not support expectations (this is the case, for example, of HTTP/1.0 servers).
  

### 101 Switching Protocols**  
  The requester has asked the server to switch protocols and the server has agreed to do so.


### 102 Processing (WebDAV; RFC 2518)**  
  A WebDAV request may contain many sub-requests involving file operations, requiring a long time to complete the request. This code indicates that the server has received and is processing the request, but no response is available yet. This prevents the client from timing out and assuming the request was lost. The status code is deprecated.


### 103 Early Hints (RFC 8297)**  
  Used to return some response headers before final HTTP message.

## 2xx Success

This class of status codes indicates the action requested by the client was received, understood, and accepted.


### 200 OK**  
  Standard response for successful HTTP requests. The actual response will depend on the request method used. In a GET request, the response will contain an entity corresponding to the requested resource. In a POST request, the response will contain an entity describing or containing the result of the action.


### 201 Created**  
  The request has been fulfilled, resulting in the creation of a new resource.


### 202 Accepted**  
  The request has been accepted for processing, but the processing has not been completed. The request might or might not be eventually acted upon, and may be disallowed when processing occurs.


### 203 Non-Authoritative Information (since HTTP/1.1)**  
  The server is a transforming proxy (e.g. a Web accelerator) that received a 200 OK from its origin, but is returning a modified version of the origin's response.


### 204 No Content**  
  The server successfully processed the request, and is not returning any content.


### 205 Reset Content**  
  The server successfully processed the request, asks that the requester reset its document view, and is not returning any content.


### 206 Partial Content**  
  The server is delivering only part of the resource (byte serving) due to a range header sent by the client. The range header is used by HTTP clients to enable resuming of interrupted downloads, or split a download into multiple simultaneous streams.


### 207 Multi-Status (WebDAV; RFC 4918)**  
  The message body that follows is by default an XML message and can contain a number of separate response codes, depending on how many sub-requests were made.


### 208 Already Reported (WebDAV; RFC 5842)**  
  The members of a DAV binding have already been enumerated in a preceding part of the (multistatus) response, and are not being included again.


### 226 IM Used (RFC 3229)**  
  The server has fulfilled a request for the resource, and the response is a representation of the result of one or more instance-manipulations applied to the current instance.

## 3xx Redirection

This class of status code indicates the client must take additional action to complete the request. Many of these status codes are used in URL redirection.

A user agent may carry out the additional action with no user interaction only if the method used in the second request is GET or HEAD. A user agent may automatically redirect a request. A user agent should detect and intervene to prevent cyclical redirects.


### 300 Multiple Choices**  
  Indicates multiple options for the resource from which the client may choose (via agent-driven content negotiation). For example, this code could be used to present multiple video format options, to list files with different filename extensions, or to suggest word-sense disambiguation.


### 301 Moved Permanently**  
  This and all future requests should be directed to the given URI.


### 302 Found (Previously "Moved temporarily")**  
  Tells the client to look at (browse to) another URL. The HTTP/1.0 specification required the client to perform a temporary redirect with the same method (the original describing phrase was "Moved Temporarily"), but popular browsers implemented 302 redirects by changing the method to GET. Therefore, HTTP/1.1 added status codes 303 and 307 to distinguish between the two behaviours.


### 303 See Other (since HTTP/1.1)**  
  The response to the request can be found under another URI using the GET method. When received in response to a POST (or PUT/DELETE), the client should presume that the server has received the data and should issue a new GET request to the given URI.


### 304 Not Modified**  
  Indicates that the resource has not been modified since the version specified by the request headers `If-Modified-Since` or `If-None-Match`. In such case, there is no need to retransmit the resource since the client still has a previously-downloaded copy.


### 305 Use Proxy (since HTTP/1.1)**  
  The requested resource is available only through a proxy, the address for which is provided in the response. For security reasons, many HTTP clients (such as Mozilla Firefox and Internet Explorer) do not obey this status code.


### 306 Switch Proxy**  
  No longer used. Originally meant "Subsequent requests should use the specified proxy."


### 307 Temporary Redirect (since HTTP/1.1)**  
  In this case, the request should be repeated with another URI; however, future requests should still use the original URI. In contrast to how 302 was historically implemented, the request method is not allowed to be changed when reissuing the original request. For example, a POST request should be repeated using another POST request.


### 308 Permanent Redirect**  
  This and all future requests should be directed to the given URI. 308 parallels the behavior of 301, but does not allow the HTTP method to change. So, for example, submitting a form to a permanently redirected resource may continue smoothly.

# 4xx Client Errors

## 404 Error on Wikimedia

This class of status code is intended for situations in which the error seems to have been caused by the client. Except when responding to a HEAD request, the server should include an entity containing an explanation of the error situation, and whether it is a temporary or permanent condition. These status codes are applicable to any request method. User agents should display any included entity to the user.

### 400 Bad Request
The server cannot or will not process the request due to an apparent client error (e.g., malformed request syntax, size too large, invalid request message framing, or deceptive request routing).

### 401 Unauthorized
Similar to 403 Forbidden, but specifically for use when authentication is required and has failed or has not yet been provided. The response must include a `WWW-Authenticate` header field containing a challenge applicable to the requested resource.

### 402 Payment Required
Reserved for future use. Some examples of its usage include:
- Google Developers API uses this status if a particular developer has exceeded the daily limit on requests.
- Sipgate uses this code if an account does not have sufficient funds to start a call.
- Shopify uses this code when a store has not paid their fees and is temporarily disabled.
- Stripe uses this code for failed payments where parameters were correct (e.g., blocked fraudulent payments).

### 403 Forbidden
The request contained valid data and was understood by the server, but the server is refusing action. Possible reasons include insufficient permissions or an attempted prohibited action.

### 404 Not Found
The requested resource could not be found but may be available in the future. Subsequent requests by the client are permissible.

### 405 Method Not Allowed
A request method is not supported for the requested resource; for example, a GET request on a form that requires POST.

### 406 Not Acceptable
The requested resource is capable of generating only content not acceptable according to the `Accept` headers sent in the request.

### 407 Proxy Authentication Required
The client must first authenticate itself with the proxy.

### 408 Request Timeout
The server timed out waiting for the request. The client may repeat the request without modifications at any later time.

### 409 Conflict
Indicates that the request could not be processed because of conflict in the current state of the resource.

### 410 Gone
Indicates that the resource requested was previously in use but is no longer available and will not be available again.

### 411 Length Required
The request did not specify the length of its content, which is required by the requested resource.

### 412 Precondition Failed
The server does not meet one of the preconditions that the requester put on the request header fields.

### 413 Payload Too Large
The request is larger than the server is willing or able to process. Previously called "Request Entity Too Large."

### 414 URI Too Long
The URI provided was too long for the server to process.

### 415 Unsupported Media Type
The request entity has a media type which the server or resource does not support.

### 416 Range Not Satisfiable
The client has asked for a portion of the file (byte serving), but the server cannot supply that portion.

### 417 Expectation Failed
The server cannot meet the requirements of the `Expect` request-header field.

### 418 I'm a teapot
Defined in RFC 2324 as a joke response for teapots requested to brew coffee.

### 421 Misdirected Request
The request was directed at a server that is not able to produce a response.

### 422 Unprocessable Content
The request was well-formed (i.e., syntactically correct) but could not be processed.

### 423 Locked (WebDAV)
The resource that is being accessed is locked.

### 424 Failed Dependency (WebDAV)
The request failed because it depended on another request and that request failed.

### 425 Too Early
Indicates that the server is unwilling to risk processing a request that might be replayed.

### 426 Upgrade Required
The client should switch to a different protocol such as TLS/1.3.

### 428 Precondition Required
The origin server requires the request to be conditional to prevent the 'lost update' problem.

### 429 Too Many Requests
The user has sent too many requests in a given amount of time. Used for rate-limiting.

### 431 Request Header Fields Too Large
The server is unwilling to process the request because the header fields are too large.

### 451 Unavailable For Legal Reasons
A server operator has received a legal demand to deny access to a resource.

---

# 5xx Server Errors

The server failed to fulfill a request. Response status codes beginning with "5" indicate that the server is aware it has encountered an error.

### 500 Internal Server Error
A generic error message, given when an unexpected condition was encountered and no more specific message is suitable.

### 501 Not Implemented
The server either does not recognize the request method or lacks the ability to fulfill the request.

### 502 Bad Gateway
The server was acting as a gateway or proxy and received an invalid response from the upstream server.

### 503 Service Unavailable
The server cannot handle the request (because it is overloaded or down for maintenance). Generally, this is a temporary state.

### 504 Gateway Timeout
The server was acting as a gateway or proxy and did not receive a timely response from the upstream server.

### 505 HTTP Version Not Supported
The server does not support the HTTP version used in the request.

### 506 Variant Also Negotiates
Transparent content negotiation for the request results in a circular reference.

### 507 Insufficient Storage (WebDAV)
The server is unable to store the representation needed to complete the request.

### 508 Loop Detected (WebDAV)
The server detected an infinite loop while processing the request.

### 510 Not Extended
Further extensions to the request are required for the server to fulfill it.

### 511 Network Authentication Required
The client needs to authenticate to gain network access. Used by intercepting proxies controlling network access.
