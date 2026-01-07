# PARSING BODY

## Transfer Encoding

HTTP Transfer-coding : case insensitive
HTTP Transfer-coding name : should be in name registry

### Chunked Transfer Encoding

- Content is wrapped so that it can be sent in chunks
- Content is transferred as a sequence of length-delimited buffers
- Enables the sender to retain connection persistence
- Enables recipient to know when it has received the entire message.

`
  chunked-body   = *chunk
                   last-chunk
                   trailer-section
                   CRLF

  chunk          = chunk-size [ chunk-ext ] CRLF
                   chunk-data CRLF
  chunk-size     = 1*HEXDIG
  last-chunk     = 1*("0") [ chunk-ext ] CRLF

  chunk-data     = 1*OCTET ; a sequence of chunk-size octets

