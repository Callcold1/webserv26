# nginx-style HTTP Configuration Scopes

This document describes **which configuration directives are typically global, server-scoped, or location-scoped** in an HTTP server like **nginx**, and which directives can appear in **multiple contexts**.

This is especially useful when designing or re-implementing an HTTP server (e.g. a C++98 HTTP/1.1 server).

---

## 1. Configuration hierarchy

nginx configuration contexts are hierarchical:

    main (global)
     └── http
     └── server
     └── location


Rules:
- Inner scopes **inherit** from outer scopes unless overridden
- Not every directive is valid in every scope
- Some directives are intentionally restricted for correctness and performance

---

## 2. Directive placement table

| Directive / Variable | Global (main) | http | server | location |
|---------------------|---------------|------|--------|----------|
| `worker_processes` | ✅ | ❌ | ❌ | ❌ |
| `events {}` | ✅ | ❌ | ❌ | ❌ |
| `http {}` | ✅ | ❌ | ❌ | ❌ |
| `include` | ✅ | ✅ | ✅ | ✅ |
| `error_log` | ✅ | ✅ | ✅ | ❌ |
| `pid` | ✅ | ❌ | ❌ | ❌ |
| `worker_connections` | ❌ | ❌ | ❌ | ❌ *(events only)* |
| `sendfile` | ❌ | ✅ | ✅ | ❌ |
| `keepalive_timeout` | ❌ | ✅ | ✅ | ❌ |
| `client_max_body_size` | ❌ | ✅ | ✅ | ✅ |
| `server {}` | ❌ | ✅ | ❌ | ❌ |
| `listen` | ❌ | ❌ | ✅ | ❌ |
| `server_name` | ❌ | ❌ | ✅ | ❌ |
| `root` | ❌ | ❌ | ✅ | ✅ |
| `index` | ❌ | ❌ | ✅ | ✅ |
| `location {}` | ❌ | ❌ | ✅ | ❌ |
| `try_files` | ❌ | ❌ | ❌ | ✅ |
| `proxy_pass` | ❌ | ❌ | ❌ | ✅ |
| `cgi_pass` / `fastcgi_pass` | ❌ | ❌ | ❌ | ✅ |
| `return` | ❌ | ❌ | ❌ | ✅ |
| `rewrite` | ❌ | ❌ | ❌ | ✅ |
| `autoindex` | ❌ | ❌ | ✅ | ✅ |
| `limit_except` | ❌ | ❌ | ❌ | ✅ |
| `allow` / `deny` | ❌ | ❌ | ❌ | ✅ |

---

## 3. Scope-by-scope explanation

### Global (`main`) scope
**Purpose:** Process-level and OS-level behavior

Typical directives:
- `worker_processes`
- `pid`
- `error_log`
- `events {}` block
- `http {}` block
- `include`

Use this scope for:
- process count
- logging setup
- event loop strategy
- global limits

---

### `http` scope
**Purpose:** HTTP protocol defaults

Typical directives:
- `sendfile`
- `keepalive_timeout`
- `client_max_body_size`
- `default_type`
- `log_format`
- `access_log`
- `gzip`

Applies to all virtual hosts unless overridden.

---

### `server` scope
**Purpose:** Virtual hosts and socket binding

Typical directives:
- `listen`
- `server_name`
- `root`
- `index`
- `error_page`
- `client_max_body_size`
- `autoindex`

Each `server` block usually maps to one virtual host.

---

### `location` scope
**Purpose:** URI-based routing and request handling

Typical directives:
- `root` (override)
- `try_files`
- `return`
- `rewrite`
- `proxy_pass`
- `cgi_pass`
- `limit_except`
- `autoindex`

This is where request semantics are decided.

---

## 4. Directives valid in multiple scopes

| Directive | http | server | location | Notes |
|---------|------|--------|----------|------|
| `root` | ❌ | ✅ | ✅ | Location overrides server |
| `index` | ❌ | ✅ | ✅ | Used for directories |
| `client_max_body_size` | ✅ | ✅ | ✅ | Commonly overridden |
| `autoindex` | ❌ | ✅ | ✅ | Directory listing |
| `error_page` | ❌ | ✅ | ❌ | Server-specific |
| `access_log` | ✅ | ✅ | ❌ | Can override per server |

**Inheritance rule:**  
The closest (most specific) definition wins.

---

## 5. Design rationale

nginx restricts directive placement to:
- Avoid ambiguous behavior
- Prevent expensive per-request logic
- Keep inheritance predictable
- Separate lifecycle, protocol, and request concerns

---

## 6. Mapping to a custom HTTP server design

| nginx concept | Typical custom server structure |
|--------------|---------------------------------|
| `main` | `Config` / `RuntimeConfig` |
| `http` | `HttpConfig` |
| `server` | `ServerConfig` |
| `location` | `LocationConfig` |
| inheritance | parent pointer / reference |
| override | nearest-match lookup |

---

This structure scales cleanly and maps well to a non-blocking, poll/epoll-based HTTP/1.1 server.
