## Purpose
This document explains the reasoning behind the server’s HTTP versioning strategy and RFC layering. It is intended to justify design decisions during review, defense, or future maintenance.

---

## High‑level Strategy
The server implements HTTP as a **layered protocol**, separating *vocabulary* (what exists) from *behavior* (how it must act). This mirrors the historical evolution of HTTP and avoids coupling the implementation to outdated or ambiguous rules.

The approach is:
1. **HTTP/1.0 compatibility** based on RFC 1945
2. **HTTP/1.1 feature completeness** based on RFC 2616
3. **Authoritative behavior and framing rules** based on RFC 9110 / RFC 9112

---

## Why RFC 1945 is used for HTTP/1.0
RFC 1945 is the final, widely‑implemented description of HTTP/1.0. It provides:
- The correct status codes and header vocabulary
- Historically accurate client expectations
- Conservative, single‑request connection semantics

HTTP/1.0 is treated strictly as a **compatibility mode**, not a first‑class protocol. The server prioritizes correctness and robustness over optional extensions (e.g. keep‑alive).

---

## Why RFC 2616 is used for HTTP/1.1 features
RFC 2616 serves as a **complete catalog of HTTP/1.1 concepts**, including:
- Methods and their intended semantics
- Standard headers and status codes
- High‑level request/response structure

Although RFC 2616 contains ambiguities and deprecated behavior, it remains the best single reference for understanding *what HTTP/1.1 is supposed to support*.

The server uses RFC 2616 as a **feature inventory**, not as a strict behavioral authority.

---

## Why RFC 9110 / RFC 9112 override behavior
RFC 9110 and RFC 9112 represent over two decades of operational experience and security hardening. They:
- Resolve ambiguities present in RFC 2616
- Define strict and unambiguous message framing rules
- Close request smuggling and desynchronization edge cases

Whenever RFCs conflict:
> **RFC 9110 / RFC 9112 rules always take precedence.**

This ensures that the server behaves correctly under modern traffic patterns while remaining backward compatible.

---

## Resulting Benefits
- Clean separation of parsing, semantics, and transport
- Predictable behavior across HTTP versions
- Easier auditing and reasoning about correctness
- Compatibility with real‑world clients and proxies

This design avoids rewriting the server as standards evolve and allows incremental tightening of rules without architectural changes.

