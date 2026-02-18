# URI

## 4. Identifiers in HTTP

Uniform Resource Identifiers (URIs) [URI] are used throughout HTTP as the means for identifying resources

### 4.1. URI References

URI references are used to target requests, indicate redirects, and define relationships.

An "absolute-path" rule is defined for protocol elements that can contain a non-empty path component.  
A "partial-URI" rule is defined for protocol elements that can contain a relative URI but **not** a fragment component.

   | URI type      | Further Description      |
   |---------------|--------------------------|
   | URI-reference | see [URI], Section 4.1   |
   | absolute-URI  | see [URI], Section 4.3   |
   | relative-part | see [URI], Section 4.2   |
   | authority     | see [URI], Section 3.2   |
   | uri-host      | see [URI], Section 3.2.2 |
   | port          | see [URI], Section 3.2.3 |
   | path-abempty  | see [URI], Section 3.3   |
   | segment       | see [URI], Section 3.3   |
   | query         | see [URI], Section 3.4   |

   - absolute-path = 1*( "/" segment )
   - partial-URI   = relative-part [ "?" query ]  

Each protocol element in HTTP that allows a URI reference will indicate in its ABNF production whether the element allows

   - any form of reference (URI-reference)
   - only a URI in absolute form (absolute-URI)
   - only the path and optional query components (partial-URI)
   - some combination of the above.

Unless otherwise indicated, URI references are parsed relative to the target URI (Section 7.1). It is RECOMMENDED that all senders and recipients support, at a minimum, URIs with lengths of 8000 octets in protocol elements. Note that this implies some structures and on-wire representations (for example, the request line in HTTP/1.1) will necessarily be larger in some cases.

### 4.2. HTTP-Related URI Schemes

IANA maintains the registry of URI Schemes [BCP35] at <https://www.iana.org/assignments/uri-schemes/>. Although requests might target any URI scheme, the following schemes are inherent to HTTP servers:

|Table 2 | URI Scheme |  Section |
|-------|-------------|---------|
| http 	| Hypertext Transfer Protocol | 4.2.1 |
| https 	| Hypertext Transfer Protocol Secure |	4.2.2 |

- Note that the presence of an "http" or "https" URI does not imply that there is always an HTTP server at the identified origin listening for connections.
- Anyone can mint a URI, whether or not a server exists and whether or not that server currently maps that identifier to a resource.  
- The delegated nature of registered names and IP addresses creates a federated namespace whether or not an HTTP server is present.

#### 4.2.1. HTTP URI Scheme

The "http" URI scheme is hereby defined for minting identifiers within the hierarchical namespace governed by a potential HTTP origin server listening for TCP connections on a given port.

    http-URI = "http" "://" authority path-abempty [ "?" query ]

The origin server for an "http" URI is identified by the authority component  

The authority component includes : 

    a host identifier
    an optional port number

If the port subcomponent is empty or not given, TCP port 80 (the reserved port for WWW services) is the default.  

The origin determines who has the right to respond authoritatively to requests that target the identified resource, as defined in Section 4.3.2.

A sender MUST NOT generate an "http" URI with an empty host identifier. A recipient that processes such a URI reference MUST reject it as invalid.  

The hierarchical path component and optional query component identify the target resource within that origin server's namespace.

#### 4.2.2. https URI Scheme

The "https" URI scheme is hereby defined for minting identifiers within the hierarchical namespace governed by a potential origin server listening for TCP connections on a given port and capable of establishing a TLS ([TLS13]) connection that has been secured for HTTP communication. In this context, "secured" specifically means that the server has been authenticated as acting on behalf of the identified authority and all HTTP communication with that server has confidentiality and integrity protection that is acceptable to both client and server.

  https-URI = "https" "://" authority path-abempty [ "?" query ]

The origin server for an "https" URI is identified by the authority component, which includes a host identifier ([URI], Section 3.2.2) and optional port number ([URI], Section 3.2.3). If the port subcomponent is empty or not given, TCP port 443 (the reserved port for HTTP over TLS) is the default. The origin determines who has the right to respond authoritatively to requests that target the identified resource, as defined in Section 4.3.3. A sender MUST NOT generate an "https" URI with an empty host identifier. A recipient that processes such a URI reference MUST reject it as invalid. The hierarchical path component and optional query component identify the target resource within that origin server's namespace. A client MUST ensure that its HTTP requests for an "https" resource are secured, prior to being communicated, and that it only accepts secured responses to those requests. Note that the definition of what cryptographic mechanisms are acceptable to client and server are usually negotiated and can change over time. Resources made available via the "https" scheme have no shared identity with the "http" scheme. They are distinct origins with separate namespaces. However, extensions to HTTP that are defined as applying to all origins with the same host, such as the Cookie protocol [COOKIE], allow information set by one service to impact communication with other services within a matching group of host domains. Such extensions ought to be designed with great care to prevent information obtained from a secured connection being inadvertently exchanged within an unsecured context.

#### 4.2.3. http(s) Normalization and Comparison

URIs with an "http" or "https" scheme are normalized and compared according to the methods defined in Section 6 of [URI], using the defaults described above for each scheme.
HTTP does not require the use of a specific method for determining equivalence. For example, a cache key might be compared as a simple string, after syntax-based normalization, or after scheme-based normalization. Scheme-based normalization (Section 6.2.3 of [URI]) of "http" and "https" URIs involves the following additional rules:
If the port is equal to the default port for a scheme, the normal form is to omit the port subcomponent.
When not being used as the target of an OPTIONS request, an empty path component is equivalent to an absolute path of "/", so the normal form is to provide a path of "/" instead.The scheme and host are case-insensitive and normally provided in lowercase; all other components are compared in a case-sensitive manner.Characters other than those in the "reserved" set are equivalent to their percent-encoded octets: the normal form is to not encode them (see Sections 2.1 and 2.2 of [URI]). For example, the following three URIs are equivalent:

    http://example.com:80/~smith/home.html
    http://EXAMPLE.com/%7Esmith/home.html
    http://EXAMPLE.com:/%7esmith/home.html

Two HTTP URIs that are equivalent after normalization (using any method) can be assumed to identify the same resource, and any HTTP component MAY perform normalization. As a result, distinct resources SHOULD NOT be identified by HTTP URIs that are equivalent after normalization (using any method defined in Section 6.2 of [URI]).

##### 4.2.4. Deprecation of userinfo in http(s) URIs

The URI generic syntax for authority also includes a userinfo subcomponent ([URI], Section 3.2.1) for including user authentication information in the URI. In that subcomponent, the use of the format "user:password" is deprecated.
Some implementations make use of the userinfo component for internal configuration of authentication information, such as within command invocation options, configuration files, or bookmark lists, even though such usage might expose a user identifier or password. A sender MUST NOT generate the userinfo subcomponent (and its "@" delimiter) when an "http" or "https" URI reference is generated within a message as a target URI or field value. Before making use of an "http" or "https" URI reference received from an untrusted source, a recipient SHOULD parse for userinfo and treat its presence as an error; it is likely being used to obscure the authority for the sake of phishing attacks.
4.2.5. http(s) References with Fragment Identifiers

Fragment identifiers allow for indirect identification of a secondary resource, independent of the URI scheme, as defined in Section 3.5 of [URI]. Some protocol elements that refer to a URI allow inclusion of a fragment, while others do not. They are distinguished by use of the ABNF rule for elements where fragment is allowed; otherwise, a specific rule that excludes fragments is used.

Note: The fragment identifier component is not part of the scheme definition for a URI scheme (see Section 4.3 of [URI]), thus does not appear in the ABNF definitions for the "http" and "https" URI schemes above.

### 4.3. Authoritative Access

Authoritative access refers to dereferencing a given identifier, for the sake of access to the identified resource, in a way that the client believes is authoritative (controlled by the resource owner). The process for determining whether access is granted is defined by the URI scheme and often uses data within the URI components, such as the authority component when the generic syntax is used. However, authoritative access is not limited to the identified mechanism.
Section 4.3.1 defines the concept of an origin as an aid to such uses, and the subsequent subsections explain how to establish that a peer has the authority to represent an origin. See Section 17.1 for security considerations related to establishing authority.

#### 4.3.1. URI Origin

The "origin" for a given URI is the triple of `scheme`, `host`, and `port` after **normalizing** the scheme and host to **lowercase** and normalizing the port to **remove any leading zeros**. If port is elided from the URI, the default port for that scheme is used. For example, the URI

   https://Example.Com/happy.js

would have the origin

   { "https", "example.com", "443" }

which can also be described as the normalized URI prefix with port always present:

   https://example.com:443

**
Each origin defines its own **namespace** and controls how **identifiers** within that namespace are **mapped** to **resources**. In turn, how the origin responds to valid requests, consistently over time, determines the semantics that users will associate with a URI, and the usefulness of those semantics is what ultimately transforms these mechanisms into a resource for users to reference and access in the future. Two origins are **distinct** if they differ in scheme, host, or port. Even when it can be verified that the same entity controls two distinct origins, the two namespaces under those origins are distinct unless **explicitly aliased** by a server authoritative for that origin. Origin is also used within HTML and related Web protocols, beyond the scope of this document, as described in [RFC6454].
 
#### 4.3.2. http Origins

Although HTTP is **independent** of the **transport protocol**, the "http" scheme (Section 4.2.1) is specific to *associating authority with whomever controls the origin server listening for TCP connections on the indicated port of whatever host is identified within the authority component*. This is a very weak sense of authority because it depends on both client-specific name resolution mechanisms and communication that might not be secured from an on-path attacker. Nevertheless, it is a sufficient minimum for binding "http" identifiers to an origin server for consistent resolution within a trusted environment.

If the `host identifier` is provided as an `IP address`, the origin server is the `listener` (if any) on the indicated `TCP port` at that **IP address**. 

If `host` is a **registered name**, the registered name is an indirect identifier for use with a **name resolution service**, such as DNS, to find an address for an appropriate origin server. 

When an "http" URI is used within a context that calls for access to the indicated resource, a client MAY attempt access by **resolving the host identifier** to an **IP address**, establishing a TCP connection to that address on the indicated port, and sending over that connection an HTTP request message containing a request target that matches the client's target URI (Section 7.1). 

If the server responds to such a request with a non-interim HTTP response message, as described in Section 15, then that response is considered an authoritative answer to the client's request. Note, however, that the above is not the only means for obtaining an authoritative response, nor does it imply that an authoritative response is always necessary (see [CACHING]). For example, the Alt-Svc header field [ALTSVC] allows an origin server to identify other services that are also authoritative for that origin. Access to "http" identified resources might also be provided by protocols outside the scope of this document.

#### 4.3.3. https Origins

The "https" scheme (Section 4.2.2) associates `authority` based on the ability of a server to use the `private key` corresponding to a `certificate` that the client considers to be trustworthy for the identified origin server. The client usually relies upon a chain of trust, conveyed from some prearranged or configured trust anchor, to deem a certificate trustworthy (Section 4.3.4).

In HTTP/1.1 and earlier, a client will only attribute authority to a server when they` are communicating over a successfully established and secured connection specifically to that URI origin's host.  

The connection establishment and certificate verification are used as proof of authority. In HTTP/2 and HTTP/3, a client will attribute authority to a server when they are communicating over a successfully established and secured connection if the URI origin's host matches any of the hosts present in the server's certificate and the client believes that it could open a connection to that host for that URI.  

In practice, a client will make a DNS query to check that the origin's host contains the same server IP address as the established connection. This restriction can be removed by the origin server sending an equivalent ORIGIN frame [RFC8336]. 

The 'request target's' 'host' and 'port value' are passed within each `HTTP request`, identifying the `origin` and distinguishing it from other `namespaces` that might be controlled by the same server (Section 7.2). It is the origin's responsibility to ensure that any services provided with control over its certificate's `private key` are equally responsible for managing the corresponding "https" namespaces or at least prepared to reject requests that appear to have been misdirected (Section 7.4). 

An origin server might be unwilling to process requests for certain target URIs even when they have the authority to do so. For example, when a host operates distinct services on different ports (e.g., 443 and 8000), checking the target URI at the origin server is necessary (even after the connection has been secured) because a network attacker might cause connections for one port to be received at some other port. 

Failing to check the target URI might allow such an attacker to replace a response to one target URI (e.g., "https://example.com/foo") with a seemingly authoritative response from the other port (e.g., "https://example.com:8000/foo"). 

Note that the "https" scheme does not rely on TCP and the connected port number for associating authority, since both are outside the secured communication and thus cannot be trusted as definitive. 

Hence, the HTTP communication might take place over any channel that has been secured, as defined in Section 4.2.2, including protocols that don't use TCP. 

When an "https" URI is used within a context that calls for access to the indicated resource, a client MAY attempt access by resolving the host identifier to an IP address, establishing a TCP connection to that address on the indicated port, securing the connection end-to-end by successfully initiating TLS over TCP with confidentiality and integrity protection, and sending over that connection an HTTP request message containing a request target that matches the client's target URI (Section 7.1). 

If the server responds to such a request with a non-interim HTTP response message, as described in Section 15, then that response is considered an authoritative answer to the client's request. Note, however, that the above is not the only means for obtaining an authoritative response, nor does it imply that an authoritative response is always necessary (see [CACHING]).

### 4.3.4. https Certificate Verification

To establish a secured connection to dereference a URI, a client MUST verify that the service's identity is an acceptable match for the URI's origin server. Certificate verification is used to prevent server impersonation by an on-path attacker or by an attacker that controls name resolution. This process requires that a client be configured with a set of trust anchors.

In general, a client MUST verify the service identity using the verification process defined in Section 6 of [RFC6125]. The client MUST construct a reference identity from the service's host: if the host is a literal IP address (Section 4.3.5), the reference identity is an IP-ID, otherwise the host is a name and the reference identity is a DNS-ID. 

A reference identity of type CN-ID MUST NOT be used by clients. As noted in Section 6.2.1 of [RFC6125], a reference identity of type CN-ID might be used by older clients. A client might be specially configured to accept an alternative form of server identity verification. For example, a client might be connecting to a server whose address and hostname are dynamic, with an expectation that the service will present a specific certificate (or a certificate matching some externally defined reference identity) rather than one matching the target URI's origin. 

In special cases, it might be appropriate for a client to simply ignore the server's identity, but it must be understood that this leaves a connection open to active attack. 

If the certificate is not valid for the target URI's origin, a user agent MUST either obtain confirmation from the user before proceeding (see Section 3.5) or terminate the connection with a bad certificate error. Automated clients MUST log the error to an appropriate audit log (if available) and SHOULD terminate the connection (with a bad certificate error). Automated clients MAY provide a configuration setting that disables this check, but MUST provide a setting which enables it.

### 4.3.5. IP-ID Reference Identity

A server that is identified using an IP address literal in the "host" field of an "https" URI has a reference identity of type IP-ID. An IP version 4 address uses the "IPv4address" ABNF rule, and an IP version 6 address uses the "IP-literal" production with the "IPv6address" option; see Section 3.2.2 of [URI].

A reference identity of IP-ID contains the decoded bytes of the IP address.

     An IP version 4 address is 4 octets
     An IP version 6 address is 16 octets.
     
     Use of IP-ID is not defined for any other IP version. 
     
The iPAddress choice in the certificate subjectAltName extension does not explicitly include the IP version and so relies on the length of the address to distinguish versions; see Section 4.2.1.6 of [RFC5280]. A reference identity of type IP-ID matches if the address is identical to an iPAddress value of the subjectAltName extension of the certificate.

## URI TYPES AND EXAMPLES 

### 1. Absolute URI

    Example:

    http://example.com/path?query=123#fragment

    Structure:

    scheme://authority/path?query#fragment

    Components:
        Scheme (e.g., http, https, ftp)
        Authority (includes optional user info, host, and port)
        Path (resource location)
        Query (optional parameters)
        Fragment (optional internal resource reference)
    Parsing Considerations:
        The scheme is mandatory.
        The authority can be empty (e.g., mailto:user@example.com).
        Paths may be hierarchical.

    Sure! Here’s an example of an absolute URI that includes a host and port:

    https://example.com:8080/path/to/resource?query=123#section

    Breakdown of Components:

    Scheme: https
    Authority: example.com:8080
        Host: example.com
        Port: 8080
    Path: /path/to/resource
    Query: query=123 (optional)
    Fragment: #section (optional)

    This kind of URI is commonly used when a web server is running on a non-default port (e.g., 8080 instead of the default 443 for HTTPS or 80 for HTTP).

### 2. Relative URI

    Example:

/path/to/resource?query=123#fragment

Structure:

    path?query#fragment

    Components:
        Path (relative to a base URI)
        Query
        Fragment
    Parsing Considerations:
        Used when resolving links within the context of a base URI.
        No scheme or authority.
        Must be resolved against an absolute URI.

### 3. URN (Uniform Resource Name)

    Example:

urn:isbn:0451450523

Structure:

    urn:<namespace>:<resource>

    Components:
        urn: (fixed scheme)
        Namespace identifier (e.g., isbn, ietf, uuid)
        Resource-specific identifier
    Parsing Considerations:
        No authority, path, query, or fragment.
        Used for persistent, location-independent names.

### 4. Data URI

    Example:

data:text/plain;base64,SGVsbG8sIFdvcmxkIQ==

Structure:

    data:[<mediatype>][;base64],<data>

    Components:
        data: scheme
        Media type (optional; defaults to text/plain;charset=US-ASCII)
        Base64 encoding indicator (optional)
        Data content
    Parsing Considerations:
        No authority, path, query, or fragment.
        Used to embed small resources directly in documents.

### 5. Opaque URI

    Example:

mailto:user@example.com

Structure:

    scheme:data

    Components:
        Scheme
        Opaque part (everything after the scheme, no path-based hierarchy)
    Parsing Considerations:
        No authority, path, or query.
        Scheme-specific interpretation (e.g., mailto, tel, urn).

Key Differences When Parsing URIs
| Feature |	Absolute URI |	Relative URI |	URN |	Data URI |	Opaque URI |
|---------|--------------|--------------|-----|----------|-------------|
| Has Scheme |	✅ Yes |	❌ No |	✅ Yes | ✅ Yes |	✅ Yes |
| Has Authority |	✅ Sometimes	| ❌ No |	❌ No | ❌ No | ❌ No |
| Has Path | ✅ Yes |	✅ Yes | ❌ No |	❌ No | ❌ No |
| Has Query	| ✅ Optional |	✅ Optional | ❌ No | ❌ No | ❌ No |
| Has Fragment |	✅ Optional | ✅ Optional | ❌ No | ❌ No | ❌ No |
| Needs Resolution |	❌ No | ✅ Yes |	❌ No | ❌ No | ❌ No |

    Conclusion

    Absolute URIs are complete and self-contained.
    Relative URIs need a base URI to be resolved.
    URNs act as persistent identifiers without location dependencies.
    Data URIs embed resources directly.
    Opaque URIs do not follow hierarchical path structures.

## What needs to be implemented from the above when setting up a web server?

When setting up a web server, you typically need to handle multiple types of URIs, depending on how clients interact with the server. Here’s a breakdown of what you should consider:
1. Absolute URIs (With Host & Port)

    Example:

    https://example.com:443/api/data

    Why Handle It?
        Used in full requests (especially for proxy servers and reverse proxies).
        Required in HTTP/1.1 for CONNECT and some proxy-related requests.
    How to Handle?
        Ensure the server correctly parses and routes requests based on the host and port.
        If using virtual hosts, configure the server to route based on the Host header.

2. Relative URIs

    Example:

    /images/logo.png

    Why Handle It?
        Most HTTP requests use relative URIs (e.g., /api/user, /home).
        Browsers and clients resolve these against the server’s domain.
    How to Handle?
        The web server should map relative paths to the correct resources.
        Ensure proper base URI resolution when linking resources.

3. URNs (Uniform Resource Names)

    Example:

    urn:uuid:550e8400-e29b-41d4-a716-446655440000

    Why Handle It?
        Web APIs may use URNs (e.g., persistent identifiers for objects).
    How to Handle?
        Ensure your application logic supports URN resolution if needed.
        Typically not handled at the webserver level but rather by application logic.

4. Data URIs

    Example:

    data:image/png;base64,iVBORw0KGgoAAAANSUhEUg...

    Why Handle It?
        Often embedded in HTML or CSS.
        Web servers don’t process these directly but should allow them in responses.
    How to Handle?
        Ensure security measures to prevent data URI injection in user-generated content.

5. Opaque URIs

    Example:

    mailto:user@example.com

    Why Handle It?
        Used for linking external resources (email, phone, etc.).
    How to Handle?
        Web servers don’t process these directly but may generate them in responses.
        Ensure correct encoding in links.

Summary of What a Web Server Must Handle
| URI Type	| Server Handling Needed? |	Handling Approach |
|-----------|-------------------------|-------------------|
| Absolute URI | ✅ Yes (for proxies, full URL requests)	| Parse host, port, and path for routing |
| Relative URI | ✅ Yes (most common case) |	Resolve against base path and serve content |
| URNs	⚠️ Maybe (if app logic requires) | Handle in application logic (not web server) |
| Data URIs	❌ | No (handled by browsers) | Ensure proper security in responses |
| Opaque URIs	❌ | No (used in client-side links)	Ensure proper encoding when generating links |

Key Takeaways

    Web servers must handle absolute and relative URIs.
    URNs may be relevant for API identifiers but not for web server routing.
    Data and opaque URIs don’t need direct handling by the server.

If you're setting up a web server (e.g., Nginx, Apache, Express.js), focus on correctly routing absolute and relative URIs, ensuring virtual host configuration, and implementing security for various URI-based attacks. 🚀


What Happens If an Invalid Scheme Is Used?

    If a request contains an unsupported or invalid scheme, an HTTP 400 Bad Request response is expected.
    If a request is made over plain http:// but the server only allows https://, it should return HTTP 301 Moved Permanently or HTTP 403 Forbidden.

Would you like details on handling absolute URIs in request parsing?
You said:
If a request is made over plain http:// but the server only allows https://, it should return HTTP 301 Moved Permanently or HTTP 403 Forbidden. - which is most appropriate?
ChatGPT said:

The most appropriate response depends on the server's intended behavior and security policies:
1. HTTP 301 Moved Permanently (Recommended for Redirection)

    If the server wants to redirect HTTP requests to HTTPS, it should return:

    HTTP/1.1 301 Moved Permanently
    Location: https://example.com/somepage

    This tells the client (including browsers and crawlers) that:
        The resource has permanently moved to HTTPS.
        Future requests should always use HTTPS.
        Browsers may automatically update bookmarks and caches.
    This is the best option for user experience, as it helps enforce HTTPS while allowing clients to access the resource.

2. HTTP 403 Forbidden (Strict Security Enforcement)

    If the server strictly disallows HTTP requests and does not want to redirect, it should return:

    HTTP/1.1 403 Forbidden

    This means:
        The server refuses to serve the request over HTTP.
        The client must manually switch to HTTPS.
        It provides stronger security but does not assist clients in upgrading.
    This is useful for APIs or highly secure applications that want to reject insecure connections outright.

Which One Should You Use?
Scenario	Use 301 Moved Permanently	Use 403 Forbidden
You want to redirect users to HTTPS automatically	✅ Yes	❌ No
You want strict security and force users to manually switch to HTTPS	❌ No	✅ Yes
The server hosts a public website (e.g., blog, e-commerce)	✅ Yes	❌ No
The server hosts an API or private system where HTTPS is mandatory	❌ No	✅ Yes

Conclusion:

    For websites, use 301 Moved Permanently to ensure a smooth transition to HTTPS.
    For APIs or security-sensitive applications, use 403 Forbidden to enforce HTTPS strictly.
