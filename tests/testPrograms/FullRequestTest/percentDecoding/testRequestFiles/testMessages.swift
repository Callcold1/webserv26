// 1. Space encoded as %20 in URI
"GET /hello%20world HTTP/1.1\r\nHost: example.com\r\n\r\n"

// 2. Slash encoded as %2F (URI remains valid, though decoded as '/')
"GET /folder%2Ffile HTTP/1.1\r\nHost: test.com\r\n\r\n"

// 3. Query parameter with percent-encoded space
"GET /search?q=open%20ai HTTP/1.1\r\nHost: engine.com\r\n\r\n"

// 4. Percent in value
"GET /percent?value=100%25 HTTP/1.1\r\nHost: percent.com\r\n\r\n"

// 5. Valid UTF-8 in percent-encoded URI
"GET /%E2%9C%93 HTTP/1.1\r\nHost: unicode.com\r\n\r\n"

// 6. Encoded colon in path (unusual, but legal)
"GET /foo%3Abar HTTP/1.1\r\nHost: test.com\r\n\r\n"

// 7. Encoded semicolon in URI
"GET /file%3Bversion=1 HTTP/1.1\r\nHost: test.com\r\n\r\n"

// 8. Encoded equals sign in query
"GET /config?set%3Dvalue HTTP/1.1\r\nHost: test.com\r\n\r\n"

// 9. Header value with percent-encoded space
"GET /home HTTP/1.1\r\nHost: test.com\r\nUser-Agent: MyAgent%2F1.0\r\n\r\n"

// 10. Encoded ampersand separating query params
"GET /api?key=123%26mode=edit HTTP/1.1\r\nHost: api.com\r\n\r\n"

// 11. Mixed case hex (legal)
"GET /abc%2fDef HTTP/1.1\r\nHost: site.com\r\n\r\n"

// 12. Encoded @ symbol in URI
"GET /user%40mail.com HTTP/1.1\r\nHost: mail.com\r\n\r\n"

// 13. Encoded question mark in path
"GET /api%3Ffake=1 HTTP/1.1\r\nHost: api.com\r\n\r\n"

// 14. Path with valid percent-encoded unicode
"GET /%F0%9F%98%81 HTTP/1.1\r\nHost: emoji.com\r\n\r\n"

// 15. Encoded comma in header value
"GET / HTTP/1.1\r\nHost: site.com\r\nAccept-Language: en%2Cus\r\n\r\n"

// 16. Invalid percent sequence (%G1 — non-hex)
"GET /invalid%G1 HTTP/1.1\r\nHost: test.com\r\n\r\n"

// 17. Truncated percent sequence (%)
"GET /test% HTTP/1.1\r\nHost: test.com\r\n\r\n"

// 18. Only one hex digit after %
"GET /path%2 HTTP/1.1\r\nHost: test.com\r\n\r\n"

// 19. Percent encoding in header key (invalid syntax)
"GET / HTTP/1.1\r\nHo%73t: bad.com\r\n\r\n"

// 20. Invalid UTF-8 sequence via percent encoding
"GET /%E2%28%A1 HTTP/1.1\r\nHost: broken.com\r\n\r\n"

// 21. Mixed valid/invalid encoding
"GET /mix%20ed%2Zdata HTTP/1.1\r\nHost: mixed.com\r\n\r\n"

// 22. Percent in header value without valid hex
"GET / HTTP/1.1\r\nUser-Agent: Test%zz\r\nHost: test.com\r\n\r\n"

// 23. Percent encoding over line break
"GET /bad%0\r\n%0A HTTP/1.1\r\nHost: split.com\r\n\r\n"

// 24. Double percent signs
"GET /weird%%20path HTTP/1.1\r\nHost: weird.com\r\n\r\n"

// 25. Header injection via encoded newline
"GET / HTTP/1.1\r\nX-Header: test%0D%0AMalicious: injected\r\nHost: evil.com\r\n\r\n"

// 26. Unusual percent with trailing space
"GET /some%20thing% HTTP/1.1\r\nHost: error.com\r\n\r\n"

// 27. Percent in query key with bad sequence
"GET /api?%bad=value HTTP/1.1\r\nHost: api.com\r\n\r\n"

// 28. Invalid high bytes in encoded UTF-8
"GET /%F8%88%80%80 HTTP/1.1\r\nHost: utf8fail.com\r\n\r\n"

// 29. Encoded null byte (often forbidden)
"GET /file%00name HTTP/1.1\r\nHost: null.com\r\n\r\n"

// 30. Valid path but invalid encoding in query
"GET /path?query=%3G HTTP/1.1\r\nHost: example.com\r\n\r\n"
