# HTTP Telnet/Netcat Cheatsheet (HTTP/1.0 & HTTP/1.1)

This cheatsheet contains a variety of HTTP requests you can send via `telnet` or `nc`, along with expected outcomes based on HTTP version and common server behavior.

---

## Basic usage

**Telnet:**
```bash
 telnet <host> 80
 # type the HTTP request manually, end headers with an empty line
```

**Netcat:**
```bash
 printf "<HTTP request>\r\n\r\n" | nc <host> 80
```

---

## HTTP/1.0 Requests

| Case | Example Request | Expected Outcome | Notes |
|------|----------------|----------------|-------|
| Simple GET, no Host | `GET / HTTP/1.0` | 200 OK | Host not required in 1.0, default host served |
| GET with Host | `GET / HTTP/1.0\r\nHost: example.com` | 200 OK | Server may route if host matches, else default host |
| Multiple Host headers | `GET / HTTP/1.0\r\nHost: foo.com\r\nHost: bar.com` | Default host | 1.0 lenient; first host used or default |
| Empty Host | `GET / HTTP/1.0\r\nHost:` | Default host | Empty headers ignored; lenient |
| Absolute-form URI | `GET http://example.com/index.html HTTP/1.0` | 200 OK | Use URI authority; Host optional |
| Malformed Host | `GET / HTTP/1.0\r\nHost: @@invalid@@` | 400 Bad Request (rare) | Some servers strictly validate |

---

## HTTP/1.1 Requests

| Case | Example Request | Expected Outcome | Notes |
|------|----------------|----------------|-------|
| Simple GET, valid Host | `GET / HTTP/1.1\r\nHost: example.com` | 200 OK | Required Host header |
| Missing Host | `GET / HTTP/1.1` | 400 Bad Request | Host mandatory in 1.1 |
| Empty Host | `GET / HTTP/1.1\r\nHost:` | 400 Bad Request | Host must not be empty |
| Multiple Host headers | `GET / HTTP/1.1\r\nHost: foo.com\r\nHost: bar.com` | 400 Bad Request | Multiple headers forbidden |
| Absolute-form URI | `GET http://example.com/index.html HTTP/1.1\r\nHost: attacker.com` | 200 OK | URI authority wins; Host ignored |
| Absolute-form URI, no Host header | `GET http://example.com/index.html HTTP/1.1` | 400 Bad Request | Host header required by spec, even if URI authority exists |
| Authority-form (CONNECT) | `CONNECT example.com:443 HTTP/1.1\r\nHost: ignored.com` | 200/Connection established | Use request-target authority; Host ignored |
| Invalid Host header | `GET / HTTP/1.1\r\nHost: @@invalid@@` | 400 Bad Request | Servers reject malformed host |

---

## Edge Cases / Security-Relevant Scenarios

| Case | Example | Expected Outcome | Notes |
|------|--------|----------------|-------|
| Absolute-form + Host mismatch | `GET http://victim.com/admin HTTP/1.1\r\nHost: attacker.com` | 200 OK (RFC) or 400 Bad Request (strict server) | Proxy/backend mismatch can cause request smuggling |
| Host header not matching config | `GET / HTTP/1.1\r\nHost: unknown.com` | 400 Bad Request | HTTP/1.1 strict; HTTP/1.0 serves default host |
| Absolute-form + empty Host | `GET http://example.com/index.html HTTP/1.1\r\nHost:` | 200 OK | Authority from URI used; Host ignored |
| No server name configured | `GET / HTTP/1.1\r\nHost: unknown.com` | 400 Bad Request or default host | Implementation-dependent |

---

## Tips for Testing

1. **Removing Host header in HTTP/1.1:**
   - `curl` cannot fully omit it (spec requires it).
   - Use **HTTP/1.0**: `curl --http1.0 http://example.com/`
   - Or use raw `nc` / `telnet`.

2. **Raw HTTP/1.1 request without Host:**
```bash
printf "GET / HTTP/1.1\r\nUser-Agent: test\r\n\r\n" | nc example.com 80
```

3. **Absolute-form test:**
```http
GET http://example.com/index.html HTTP/1.1
Host: ignored.com
```
- Authority wins; Host ignored by origin server.

4. **Authority-form test (CONNECT):**
```http
CONNECT example.com:443 HTTP/1.1
Host: ignored.com
```
- Used for tunneling HTTPS through proxies.

5. **Multiple Host headers test:**
```http
GET / HTTP/1.1
Host: foo.com
Host: bar.com
```
- Servers should return `400 Bad Request`.

---

## Summary Table of Outcomes

| Scenario | HTTP/1.0 | HTTP/1.1 |
|----------|-----------|-----------|
| No Host header | OK (default host) | 400 Bad Request |
| Host header empty | OK (default host) | 400 Bad Request |
| Multiple Host headers | OK / default host | 400 Bad Request |
| Absolute-form URI | Authority used | Authority used (Host ignored) |
| Host does not match config | Default host served | 400 Bad Request (or default host depending on server) |

---
*Use this cheatsheet to test server behavior, virtual host routing, and edge cases for security testing.*

