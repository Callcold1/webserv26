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

## | Permanent Header List

| * | permanent | [RFC9110, Section 12.5.5: HTTP Semantics]|
|---|-----------|------------------------------------------|
| Accept | permanent | [RFC9110, Section 12.5.1: HTTP Semantics] |
| A-IM | permanent | [RFC 3229: Delta encoding in HTTP] |
| Accept | permanent | [RFC9110, Section 12.5.1: HTTP Semantics] |
| Accept-Additions | permanent | [RFC 2324: Hyper Text Coffee Pot Control Protocol (HTCPCP/1.0)] |
| Accept-CH | permanent | [RFC 8942, Section 3.1: HTTP Client Hints] |
| Accept-Datetime | permanent | [RFC 7089: HTTP Framework for Time-Based Access to Resource States -- Memento] |
| Accept-Encoding | permanent | [RFC9110, Section 12.5.3: HTTP Semantics] |
| Accept-Features | permanent | [RFC 2295: Transparent Content Negotiation in HTTP] |
| Accept-Language | permanent | [RFC9110, Section 12.5.4: HTTP Semantics] |
| Accept-Patch | permanent | [RFC 5789: PATCH Method for HTTP] |
| Accept-Post | permanent | [Linked Data Platform 1.0] |
| Accept-Ranges | permanent | [RFC9110, Section 14.3: HTTP Semantics] |
| Accept-Signature | permanent | [RFC 9421, Section 5.1: HTTP Message Signatures] |
| Access-Control-Allow-Credentials | permanent | [Fetch] |
| Access-Control-Allow-Headers | permanent | [Fetch] |
| Access-Control-Allow-Methods | permanent | [Fetch] |
| Access-Control-Allow-Origin | permanent | [Fetch] |
| Access-Control-Expose-Headers | permanent | [Fetch] |
| Access-Control-Max-Age | permanent | [Fetch] |
| Access-Control-Request-Headers | permanent | [Fetch] |
| Access-Control-Request-Method | permanent | [Fetch] |
| Age | permanent | [RFC9111, Section 5.1: HTTP Caching] |
| Allow | permanent | [RFC9110, Section 10.2.1: HTTP Semantics] |
| ALPN | permanent | [RFC 7639, Section 2: The ALPN HTTP Header Field] |
| Alt-Svc | permanent | [RFC 7838: HTTP Alternative Services] |
| Alt-Used | permanent | [RFC 7838: HTTP Alternative Services] |
| Alternates | permanent | [RFC 2295: Transparent Content Negotiation in HTTP] |
| Apply-To-Redirect-Ref | permanent | [RFC 4437: Web Distributed Authoring and Versioning (WebDAV) Redirect Reference Resources] |
| Authentication-Control | permanent | [RFC 8053, Section 4: HTTP Authentication Extensions for Interactive Clients] |
| Authentication-Info | permanent | [RFC9110, Section 11.6.3: HTTP Semantics] |
| Authorization | permanent | [RFC9110, Section 11.6.2: HTTP Semantics] |
| Available-Dictionary | permanent | [RFC-ietf-httpbis-compression-dictionary-19, Section 2.2: Compression Dictionary Transport] |
| Cache-Control | permanent | [RFC9111, Section 5.2] |
| Cache-Status | permanent | ist 	[RFC9211: The Cache-Status HTTP Response Header Field] |
| Cal-Managed-ID | permanent | [RFC 8607, Section 5.1: Calendaring Extensions to WebDAV (CalDAV): Managed Attachments] |
| CalDAV-Timezones | permanent | [RFC 7809, Section 7.1: Calendaring Extensions to WebDAV (CalDAV): Time Zones by Reference] |
| Capsule-Protocol | permanent | [RFC9297] |
| CDN-Cache-Control | permanent | ictionary 	[RFC9213: Targeted HTTP Cache Control] 	Cache directives targeted at content delivery network|
| CDN-Loop | permanent | [RFC 8586: Loop Detection in Content Delivery Networks (CDNs)] |
| Cert-Not-After | permanent | [RFC 8739, Section 3.3: Support for Short-Term, Automatically Renewed (STAR) Certificates in the Automated Certificate|Management Environment (ACME)] 	
| Cert-Not-Before | permanent | [RFC 8739, Section 3.3: Support for Short-Term, Automatically Renewed (STAR) Certificates in the Automated Certificate|Management Environment (ACME)] 	
| Clear-Site-Data | permanent | [Clear Site Data] |
| Client-Cert | permanent | tem 	[RFC9440, Section 2: Client-Cert HTTP Header Field] |
| Client-Cert-Chain | permanent | ist 	[RFC9440, Section 2: Client-Cert HTTP Header Field] |
| Close | permanent | [RFC9112, Section 9.6: HTTP/1.1] 	(reserved|
| Concealed-Auth-Export | permanent | tem 	[RFC9729: The Concealed HTTP Authentication Scheme] |
| Connection | permanent | [RFC9110, Section 7.6.1: HTTP Semantics] |
| Content-Digest | permanent | [RFC 9530, Section 2: Digest Fields] |
| Content-Disposition | permanent | [RFC 6266: Use of the Content-Disposition Header Field in the Hypertext Transfer Protocol (HTTP)] |
| Content-Encoding | permanent | [RFC9110, Section 8.4: HTTP Semantics] |
| Content-Language | permanent | [RFC9110, Section 8.5: HTTP Semantics] |
| Content-Length | permanent | [RFC9110, Section 8.6: HTTP Semantics] |
| Content-Location | permanent | [RFC9110, Section 8.7: HTTP Semantics] |
| Content-Range | permanent | [RFC9110, Section 14.4: HTTP Semantics] |
| Content-Security-Policy | permanent | [Content Security Policy Level 3] |
| Content-Security-Policy-Report-Only | permanent | [Content Security Policy Level 3] |
| Content-Type | permanent | [RFC9110, Section 8.3: HTTP Semantics] |
| Cookie | permanent | [RFC 6265: HTTP State Management Mechanism] |
| Cross-Origin-Embedder-Policy | permanent | tem 	[HTML] |
| Cross-Origin-Embedder-Policy-Report-Only | permanent | tem 	[HTML] |
| Cross-Origin-Opener-Policy | permanent | tem 	[HTML] |
| Cross-Origin-Opener-Policy-Report-Only | permanent | tem 	[HTML] |
| Cross-Origin-Resource-Policy | permanent | [Fetch] |
| DASL | permanent | [RFC 5323: Web Distributed Authoring and Versioning (WebDAV) SEARCH] |
| Date | permanent | [RFC9110, Section 6.6.1: HTTP Semantics] |
| DAV | permanent | [RFC 4918: HTTP Extensions for Web Distributed Authoring and Versioning (WebDAV)] |
| Delta-Base | permanent | [RFC 3229: Delta encoding in HTTP] |
| Deprecation | permanent | tem 	[RFC-ietf-httpapi-deprecation-header-09, Section 2: The Deprecation HTTP Response Header Field] |
| Depth | permanent | [RFC 4918: HTTP Extensions for Web Distributed Authoring and Versioning (WebDAV)] |
| Destination | permanent | [RFC 4918: HTTP Extensions for Web Distributed Authoring and Versioning (WebDAV)] |
| Detached-JWS | permanent | [RFC 9635: Grant Negotiation and Authorization Protocol (GNAP)] |
| Dictionary-ID | permanent | [RFC-ietf-httpbis-compression-dictionary-19, Section 2.3: Compression Dictionary Transport] |
| DPoP | permanent | [RFC9449: OAuth 2.0 Demonstrating Proof of Possession (DPoP)] |
| DPoP-Nonce | permanent | [RFC9449: OAuth 2.0 Demonstrating Proof of Possession (DPoP)] |
| Early-Data | permanent | [RFC 8470: Using Early Data in HTTP] |
| ETag | permanent | [RFC9110, Section 8.8.3: HTTP Semantics] |
| Expect | permanent | [RFC9110, Section 10.1.1: HTTP Semantics] |
| Expires | permanent | [RFC9111, Section 5.3: HTTP Caching] |
| Forwarded | permanent | [RFC 7239: Forwarded HTTP Extension] |
| From | permanent | [RFC9110, Section 10.1.2: HTTP Semantics] |
| Hobareg | permanent | [RFC 7486, Section 6.1.1: HTTP Origin-Bound Authentication (HOBA)] |
| Host | permanent | [RFC9110, Section 7.2: HTTP Semantics] |
| If | permanent | [RFC 4918: HTTP Extensions for Web Distributed Authoring and Versioning (WebDAV)] |
| If-Match | permanent | [RFC9110, Section 13.1.1: HTTP Semantics] |
| If-Modified-Since | permanent | [RFC9110, Section 13.1.3: HTTP Semantics] |
| If-None-Match | permanent | [RFC9110, Section 13.1.2: HTTP Semantics] |
| If-Range | permanent | [RFC9110, Section 13.1.5: HTTP Semantics] |
| If-Schedule-Tag-Match | permanent | [ RFC 6338: Scheduling Extensions to CalDAV] |
| If-Unmodified-Since | permanent | [RFC9110, Section 13.1.4: HTTP Semantics] |
| IM | permanent | [RFC 3229: Delta encoding in HTTP] |
| Include-Referred-Token-Binding-ID | permanent | [RFC 8473: Token Binding over HTTP] |
| Keep-Alive | permanent | [RFC 2068: Hypertext Transfer Protocol -- HTTP/1.1] |
| Label | permanent | [RFC 3253: Versioning Extensions to WebDAV: (Web Distributed Authoring and Versioning)] |
| Last-Event-ID | permanent | [HTML] |
| Last-Modified | permanent | [RFC9110, Section 8.8.2: HTTP Semantics] |
| Link | permanent | [RFC 8288: Web Linking] |
| Link-Template | permanent | [RFC 9652: The Link-Template HTTP Header Field] |
| Location | permanent | [RFC9110, Section 10.2.2: HTTP Semantics] |
| Lock-Token | permanent | [RFC 4918: HTTP Extensions for Web Distributed Authoring and Versioning (WebDAV)] |
| Max-Forwards | permanent | [RFC9110, Section 7.6.2: HTTP Semantics] |
| Memento-Datetime | permanent | [RFC 7089: HTTP Framework for Time-Based Access to Resource States -- Memento] |
| Meter | permanent | [RFC 2227: Simple Hit-Metering and Usage-Limiting for HTTP] |
| MIME-Version | permanent | [RFC9112, Appendix B.1: HTTP/1.1] |
| Negotiate | permanent | [RFC 2295: Transparent Content Negotiation in HTTP] |
| NEL | permanent | [Network Error Logging] |
| OData-EntityId | permanent | [OData Version 4.01 Part 1: Protocol][OASIS][Chet_Ensign] |
| OData-Isolation | permanent | [OData Version 4.01 Part 1: Protocol][OASIS][Chet_Ensign] |
| OData-MaxVersion | permanent | [OData Version 4.01 Part 1: Protocol][OASIS][Chet_Ensign] |
| OData-Version | permanent | [OData Version 4.01 Part 1: Protocol][OASIS][Chet_Ensign] |
| Optional-WWW-Authenticate | permanent | [RFC 8053, Section 3: HTTP Authentication Extensions for Interactive Clients] |
| Ordering-Type | permanent | [RFC 3648: Web Distributed Authoring and Versioning (WebDAV) Ordered Collections Protocol] |
| Origin | permanent | [RFC 6454: The Web Origin Concept] |
| Origin-Agent-Cluster | permanent | tem 	[HTML] |
| OSCORE | permanent | [RFC 8613, Section 11.1: Object Security for Constrained RESTful Environments (OSCORE)] |
| OSLC-Core-Version | permanent | [OASIS Project Specification 01][OASIS][Chet_Ensign] |
| Overwrite | permanent | [RFC 4918: HTTP Extensions for Web Distributed Authoring and Versioning (WebDAV)] |
| Ping-From | permanent | [HTML] |
| Ping-To | permanent | [HTML] |
| Position | permanent | [RFC 3648: Web Distributed Authoring and Versioning (WebDAV) Ordered Collections Protocol] |
| Prefer | permanent | [RFC 7240: Prefer Header for HTTP] |
| Preference-Applied | permanent | [RFC 7240: Prefer Header for HTTP] |
| Priority | permanent | ictionary 	[RFC9218: Extensible Prioritization Scheme for HTTP] |
| Proxy-Authenticate | permanent | [RFC9110, Section 11.7.1: HTTP Semantics] |
| Proxy-Authentication-Info | permanent | [RFC9110, Section 11.7.3: HTTP Semantics] |
| Proxy-Authorization | permanent | [RFC9110, Section 11.7.2: HTTP Semantics] |
| Proxy-Status | permanent | ist 	[RFC9209: The Proxy-Status HTTP Response Header Field] |
| Public-Key-Pins | permanent | [RFC 7469: Public Key Pinning Extension for HTTP] |
| Public-Key-Pins-Report-Only | permanent | [RFC 7469: Public Key Pinning Extension for HTTP] |
| Range | permanent | [RFC9110, Section 14.2: HTTP Semantics] |
| Redirect-Ref | permanent | [RFC 4437: Web Distributed Authoring and Versioning (WebDAV) Redirect Reference Resources] |
| Referer | permanent | [RFC9110, Section 10.1.3: HTTP Semantics] |
| Referrer-Policy | permanent | [Referrer Policy] 	The header name does not share the HTTP Referer header's misspelling|
| Refresh | permanent | [HTML] |
| Replay-Nonce | permanent | [RFC 8555, Section 6.5.1: Automatic Certificate Management Environment (ACME)] |
| Repr-Digest | permanent | [RFC 9530, Section 3: Digest Fields] |
| Retry-After | permanent | [RFC9110, Section 10.2.3: HTTP Semantics] |
| Schedule-Reply | permanent | [RFC 6638: Scheduling Extensions to CalDAV] |
| Schedule-Tag | permanent | [RFC 6338: Scheduling Extensions to CalDAV] |
| Sec-Purpose | permanent | [Fetch] 	Intended to replace the (not registered) Purpose and x-moz headers|
| Sec-Token-Binding | permanent | [RFC 8473: Token Binding over HTTP] |
| Sec-WebSocket-Accept | permanent | [RFC 6455: The WebSocket Protocol] |
| Sec-WebSocket-Extensions | permanent | [RFC 6455: The WebSocket Protocol] |
| Sec-WebSocket-Key | permanent | [RFC 6455: The WebSocket Protocol] |
| Sec-WebSocket-Protocol | permanent | [RFC 6455: The WebSocket Protocol] |
| Sec-WebSocket-Version | permanent | [RFC 6455: The WebSocket Protocol] |
| Server | permanent | [RFC9110, Section 10.2.4: HTTP Semantics] |
| Server-Timing | permanent | [Server Timing] |
| Set-Cookie | permanent | [RFC 6265: HTTP State Management Mechanism] |
| Signature | permanent | [RFC 9421, Section 4.2: HTTP Message Signatures] |
| Signature-Input | permanent | [RFC 9421, Section 4.1: HTTP Message Signatures] |
| SLUG | permanent | [RFC 5023: The Atom Publishing Protocol] |
| SoapAction | permanent | [Simple Object Access Protocol (SOAP) 1.1] |
| Status-URI | permanent | [RFC 2518: HTTP Extensions for Distributed Authoring -- WEBDAV] |
| Strict-Transport-Security | permanent | [RFC 6797: HTTP Strict Transport Security (HSTS)] |
| Sunset | permanent | [RFC 8594: The Sunset HTTP Header Field] |
| TCN | permanent | [RFC 2295: Transparent Content Negotiation in HTTP] |
| TE | permanent | [RFC9110, Section 10.1.4: HTTP Semantics] |
| Timeout | permanent | [RFC 4918: HTTP Extensions for Web Distributed Authoring and Versioning (WebDAV)] |
| Topic | permanent | [RFC 8030, Section 5.4: Generic Event Delivery Using HTTP Push] |
| Traceparent | permanent | [Trace Context] |
| Tracestate | permanent | [Trace Context] |
| Trailer | permanent | [RFC9110, Section 6.6.2: HTTP Semantics] |
| Transfer-Encoding | permanent | [RFC9112, Section 6.1: HTTP Semantics] |
| TTL | permanent | [RFC 8030, Section 5.2: Generic Event Delivery Using HTTP Push] |
| Upgrade | permanent | [RFC9110, Section 7.8: HTTP Semantics] |
| Urgency | permanent | [RFC 8030, Section 5.3: Generic Event Delivery Using HTTP Push] |
| Use-As-Dictionary | permanent | [RFC-ietf-httpbis-compression-dictionary-19, Section 2.1: Compression Dictionary Transport] |
| User-Agent | permanent | [RFC9110, Section 10.1.5: HTTP Semantics] |
| Variant-Vary | permanent | [RFC 2295: Transparent Content Negotiation in HTTP] |
| Vary | permanent | [RFC9110, Section 12.5.5: HTTP Semantics] |
| Via | permanent | [RFC9110, Section 7.6.3: HTTP Semantics] |
| Want-Content-Digest | permanent | [RFC 9530, Section 4: Digest Fields] |
| Want-Repr-Digest | permanent | [RFC 9530, Section 4: Digest Fields] |
| WWW-Authenticate | permanent | [RFC9110, Section 11.6.1: HTTP Semantics] |
| X-Content-Type-Options | permanent | [Fetch] |
| X-Frame-Options | permanent | [HTML] |


| **Header**               | **Syntax Rules**                                        | **Whitespace Handling**                          |
|--------------------------|---------------------------------------------------------|-------------------------------------------------|
| `Accept`                | Comma-separated list of media types                     | OWS around `,` is allowed                       |
| `Accept-Charset`        | Comma-separated list of character sets                  | OWS around `,` is allowed                       |
| `Accept-Encoding`       | Comma-separated list of encoding types                  | OWS around `,` is allowed                       |
| `Accept-Language`       | Comma-separated list of language tags                   | OWS around `,` is allowed                       |
| `Accept-Ranges`         | `none` or `bytes`                                       | No leading/trailing whitespace                  |
| `Age`                   | Integer seconds                                         | No leading/trailing whitespace                  |
| `Allow`                 | Comma-separated list of HTTP methods                    | OWS around `,` is allowed                       |
| `Authorization`         | `scheme credentials`                                    | Space after scheme must be preserved            |
| `Cache-Control`         | Comma-separated directives                              | OWS around `,` is allowed                       |
| `Connection`            | Comma-separated list of connection options              | OWS around `,` is allowed                       |
| `Content-Disposition`   | `disposition-type; parameter=value`                     | No whitespace before/after `=` in parameters    |
| `Content-Encoding`      | Comma-separated list of encoding types                  | OWS around `,` is allowed                       |
| `Content-Language`      | Comma-separated list of language tags                   | OWS around `,` is allowed                       |
| `Content-Length`        | Integer bytes                                           | No leading/trailing whitespace                  |
| `Content-Location`      | Absolute or relative URI                                | No leading/trailing whitespace                  |
| `Content-Range`         | `bytes start-end/total`                                 | No whitespace inside range                      |
| `Content-Type`          | `media-type; parameter=value`                           | No whitespace before/after `=` in parameters    |
| `Date`                  | `HTTP-date` (RFC 7231 format)                           | No leading/trailing whitespace                  |
| `ETag`                  | Quoted string                                           | Must be preserved as-is                         |
| `Expect`                | `100-continue` or other expectations                    | No leading/trailing whitespace                  |
| `Expires`               | `HTTP-date`                                             | No leading/trailing whitespace                  |
| `From`                  | Email address                                          | No leading/trailing whitespace                  |
| `Host`                  | `hostname[:port]`                                      | No leading/trailing whitespace                  |
| `If-Match`              | `*` or comma-separated ETags                            | Must preserve quoted ETags                      |
| `If-Modified-Since`     | `HTTP-date`                                             | No leading/trailing whitespace                  |
| `If-None-Match`         | `*` or comma-separated ETags                            | Must preserve quoted ETags                      |
| `If-Range`              | `HTTP-date` or ETag                                     | Must preserve quoted ETags                      |
| `If-Unmodified-Since`   | `HTTP-date`                                             | No leading/trailing whitespace                  |
| `Last-Modified`         | `HTTP-date`                                             | No leading/trailing whitespace                  |
| `Location`              | Absolute URI                                            | No leading/trailing whitespace                  |
| `Max-Forwards`         | Integer                                                 | No leading/trailing whitespace                  |
| `Pragma`                | Implementation-defined                                  | OWS may be present                              |
| `Proxy-Authenticate`    | `scheme challenge`                                      | Space after scheme must be preserved            |
| `Proxy-Authorization`   | `scheme credentials`                                    | Space after scheme must be preserved            |
| `Range`                 | `bytes=start-end` or `bytes=start-`                    | No whitespace inside range                      |
| `Referer`               | Absolute or relative URI                                | No leading/trailing whitespace                  |
| `Retry-After`           | `HTTP-date` or integer seconds                         | No leading/trailing whitespace                  |
| `Server`                | Free-form server description                           | OWS may be present                              |
| `TE`                    | `trailers` or encoding list                            | OWS around `,` is allowed                       |
| `Trailer`               | Comma-separated list of headers                        | OWS around `,` is allowed                       |
| `Transfer-Encoding`     | Comma-separated encoding types                         | OWS around `,` is allowed                       |
| `Upgrade`               | Comma-separated list of protocols                      | OWS around `,` is allowed                       |
| `User-Agent`            | Free-form user agent string                            | OWS may be present                              |
| `Vary`                  | `*` or comma-separated header names                    | OWS around `,` is allowed                       |
| `Via`                   | Comma-separated list of proxies                        | OWS around `,` is allowed                       |
| `Warning`               | `code agent "text"`                                    | OWS around `,` is allowed                       |
| `WWW-Authenticate`      | `scheme challenge`                                     | Space after scheme must be preserved            |
