# Raw HTTP Requests for Telnet/Netcat Testing

This document contains a variety of raw HTTP requests formatted with `\r\n` line endings for testing servers using **telnet** or **netcat**. It covers **HTTP/1.0**, **HTTP/1.1**, edge cases, absolute-form, authority-form, and multiple Host header scenarios.

---

## 1️⃣ HTTP/1.0 Requests

### 1a. Simple GET, no Host
```bash
printf "GET / HTTP/1.0\r\n\r\n" | nc example.com 8080
```
**Expected:** 200 OK (default host)

### 1b. GET with Host
```bash
printf "GET / HTTP/1.0\r\nHost: example.com\r\n\r\n" | nc example.com 8080
```
**Expected:** 200 OK

### 1c. Multiple Host headers
```bash
printf "GET / HTTP/1.0\r\nHost: foo.com\r\nHost: bar.com\r\n\r\n" | nc example.com 8080
```
**Expected:** Default host (lenient 1.0 behavior)

### 1d. Absolute-form URI
```bash
printf "GET http://example.com/index.html HTTP/1.0\r\n\r\n" | nc example.com 8080
```
**Expected:** 200 OK

### 1e. Empty Host header
```bash
printf "GET / HTTP/1.0\r\nHost:\r\n\r\n" | nc example.com 8080
```
**Expected:** Default host

---

## 2️⃣ HTTP/1.1 Requests

### 2a. Simple GET, valid Host
```bash
printf "GET / HTTP/1.1\r\nHost: example.com\r\n\r\n" | nc example.com 8080
```
**Expected:** 200 OK

### 2b. Missing Host
```bash
printf "GET / HTTP/1.1\r\nUser-Agent: test\r\n\r\n" | nc example.com 8080
```
**Expected:** 400 Bad Request

### 2c. Empty Host header
```bash
printf "GET / HTTP/1.1\r\nHost:\r\n\r\n" | nc example.com 8080
```
**Expected:** 400 Bad Request

### 2d. Multiple Host headers
```bash
printf "GET / HTTP/1.1\r\nHost: foo.com\r\nHost: bar.com\r\n\r\n" | nc example.com 8080
```
**Expected:** 400 Bad Request

### 2e. Absolute-form URI, Host header present
```bash
printf "GET http://example.com/index.html HTTP/1.1\r\nHost: attacker.com\r\n\r\n" | nc example.com 8080
```
**Expected:** 200 OK (URI authority used, Host ignored)

### 2f. Absolute-form URI, no Host header
```bash
printf "GET http://example.com/index.html HTTP/1.1\r\n\r\n" | nc example.com 8080
```
**Expected:** 400 Bad Request (Host required by spec)

### 2g. Authority-form (CONNECT)
```bash
printf "CONNECT example.com:443 HTTP/1.1\r\nHost: ignored.com\r\n\r\n" | nc example.com 8080
```
**Expected:** 200 / Connection established (authority used, Host ignored)

### 2h. Invalid Host header
```bash
printf "GET / HTTP/1.1\r\nHost: @@invalid@@\r\n\r\n" | nc example.com 8080
```
**Expected:** 400 Bad Request

---

## 3️⃣ Edge Cases / Security-Relevant Tests

### 3a. Absolute-form + Host mismatch
```bash
printf "GET http://victim.com/admin HTTP/1.1\r\nHost: attacker.com\r\n\r\n" | nc example.com 8080
```
**Expected:** 200 OK (RFC-compliant) or 400 Bad Request (strict server)

### 3b. Host header not matching config
```bash
printf "GET / HTTP/1.1\r\nHost: unknown.com\r\n\r\n" | nc example.com 8080
```
**Expected:** 400 Bad Request

### 3c. Absolute-form + empty Host
```bash
printf "GET http://example.com/index.html HTTP/1.1\r\nHost:\r\n\r\n" | nc example.com 8080
```
**Expected:** 200 OK (authority used)

### 3d. No server name configured
```bash
printf "GET / HTTP/1.1\r\nHost: unknown.com\r\n\r\n" | nc example.com 8080
```
**Expected:** 400 Bad Request or default host (implementation-dependent)

---

### Notes:

- Press **Enter twice** in Telnet to end headers. Each Enter sends `\r\n`.
- For exact control over headers and line endings, use **Netcat + printf**.
- Absolute-form URI requests **ignore the Host header** for routing.
- Authority-form (CONNECT) is used mainly for proxy HTTPS tunneling.
- Multiple Host headers in HTTP/1.1 should always trigger **400 Bad Request**.
- Missing Host header in HTTP/1.1 also triggers **400 Bad Request**.

