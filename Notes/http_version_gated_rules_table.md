## Purpose
This document provides a concise, implementation‑oriented rules table for handling HTTP/1.0 and HTTP/1.1 requests. It is designed to be directly translatable into code and used as a validation checklist.

---

## Version Detection
| Element | HTTP/1.0 | HTTP/1.1 |
|------|---------|---------|
| Request line | `HTTP/1.0` | `HTTP/1.1` |
| Default persistence | No | Yes |
| Host header required | No | Yes |

---

## Connection Handling
| Rule | HTTP/1.0 | HTTP/1.1 |
|----|---------|---------|
| Default connection | Close | Keep‑Alive |
| `Connection: close` | Implicit | Explicit |
| Pipelining | Not allowed | Allowed |
| Reuse socket after response | No | Yes (if valid) |

---

## Message Body Framing
| Scenario | HTTP/1.0 | HTTP/1.1 |
|-------|---------|---------|
| `Transfer‑Encoding: chunked` | ❌ Reject | ✅ Allowed |
| `Content‑Length` | Optional | Optional |
| No length indicators | EOF | Chunked or EOF (restricted) |
| Both TE and CL present | ❌ Reject | Ignore CL, use TE |

---

## Method Semantics
| Method | HTTP/1.0 | HTTP/1.1 |
|------|---------|---------|
| GET | No body | Body discouraged |
| POST | Body allowed | Body allowed |
| DELETE | Undefined | Defined |
| PUT | Undefined | Defined |

---

## Header Rules
| Rule | HTTP/1.0 | HTTP/1.1 |
|----|---------|---------|
| Header name case | Insensitive | Insensitive |
| Unknown headers | Ignore | Ignore |
| obs‑fold in requests | ❌ Reject | ❌ Reject |
| Hop‑by‑hop headers | Ignore | Strip before forwarding |

---

## Response Rules
| Rule | HTTP/1.0 | HTTP/1.1 |
|----|---------|---------|
| `Transfer‑Encoding` in response | ❌ Forbidden | ✅ Allowed |
| `Content‑Length` | Recommended | Recommended |
| Connection after response | Always close | Version‑dependent |
| Reason phrase | Used | Optional / cosmetic |

---

## Error Handling
| Condition | HTTP/1.0 | HTTP/1.1 |
|---------|---------|---------|
| Missing Host | OK | 400 Bad Request |
| Invalid framing | 400 | 400 |
| Unsupported TE | 400 | 501 or 400 |

---

## Implementation Note
These rules should be applied **after parsing but before routing or I/O decisions**. The version gate must be a single, centralized decision point to avoid duplicated logic and subtle inconsistencies.

