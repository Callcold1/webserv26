// 3. Valid - simple chunked request
"POST /upload HTTP/1.1\r\nHost: example.com\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nWiki\r\n5\r\npedia\r\n0\r\n\r\n"

ACTION : NONE

fd  = 3
========
Status               : P DONE
Method               : POST
URI                  : /upload
Version              : HTTP/1.1
Header               : host
  Value              : example.com

Header               : transfer-encoding
  Value              : chunked

Body                 : pedia
ErrMess              : Parsing Complete
ErrNum               : 200

Unparsed Length      : 0

Unparsed Data        : 

// 4. Valid - empty body
"POST /empty HTTP/1.1\r\nHost: test.com\r\nTransfer-Encoding: chunked\r\n\r\n0\r\n\r\n"

ACTION : NONE

fd  = 4
========
Status               : P DONE
Method               : POST
URI                  : /empty
Version              : HTTP/1.1
Header               : host
  Value              : test.com

Header               : transfer-encoding
  Value              : chunked

Body                 : 
ErrMess              : Parsing Complete
ErrNum               : 200

Unparsed Length      : 5

Unparsed Data        : 
0


// 5. Valid - chunk extensions
"POST /data HTTP/1.1\r\nHost: server.com\r\nTransfer-Encoding: chunked\r\n\r\nA;ext=value\r\nHelloWorld\r\n0\r\n\r\n"

ACTION : PARSE CHUNK EXTENSTIONS?

fd  = 5
========
Status               : P DONE
Method               : POST
URI                  : /data
Version              : HTTP/1.1
Header               : host
  Value              : server.com

Header               : transfer-encoding
  Value              : chunked

Body                 : HelloWorld
ErrMess              : Parsing Complete
ErrNum               : 200

Unparsed Length      : 0

Unparsed Data        : 

// 6. Valid - trailing headers
"POST /log HTTP/1.1\r\nHost: myhost.com\r\nTransfer-Encoding: chunked\r\n\r\n3\r\nabc\r\n3\r\ndef\r\n0\r\nTrailer-Header: value\r\n\r\n"

ACTION : PARSE CHUNK TRAILERS?

fd  = 6
========
Status               : P ERROR
Method               : POST
URI                  : /log
Version              : HTTP/1.1
Header               : host
  Value              : myhost.com

Header               : transfer-encoding
  Value              : chunked

Body                 : def
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 2

Unparsed Data        : 


// 7. Valid - multiple small chunks
"POST /small HTTP/1.1\r\nHost: test.org\r\nTransfer-Encoding: chunked\r\n\r\n1\r\na\r\n1\r\nb\r\n1\r\nc\r\n0\r\n\r\n"

ACTION : SHOULDN'T BE AN ERROR

fd  = 7
========
Status               : P DONE
Method               : POST
URI                  : /small
Version              : HTTP/1.1
Header               : host
  Value              : test.org

Header               : transfer-encoding
  Value              : chunked

Body                 : c
ErrMess              : Parsing Complete
ErrNum               : 200

Unparsed Length      : 0

Unparsed Data        : 

// 8. Invalid - no final 0-sized chunk
"POST /bad HTTP/1.1\r\nHost: badhost\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n"

ACTION : NONE - RELYING ON SERVER TIMEOUT

fd  = 8
========
Status               : P ERROR
Method               : POST
URI                  : /bad
Version              : HTTP/1.1
Header               : host
  Value              : badhost

Header               : transfer-encoding
  Value              : chunked

Body                 : hello
ErrMess              : Read Size of Zero : Incomplete message
ErrNum               : 400

Unparsed Length      : 10

Unparsed Data        : 
5
hello

// 9. Invalid - chunk size not in hex
"POST /bad2 HTTP/1.1\r\nHost: test\r\nTransfer-Encoding: chunked\r\n\r\nZZ\r\nfail\r\n0\r\n\r\n"

ACTION : NONE

fd  = 9
========
Status               : P ERROR
Method               : POST
URI                  : /bag
Version              : HTTP/1.1
Header               : host
  Value              : test

Header               : transfer-encoding
  Value              : chunked

Body                 : 
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 15

Unparsed Data        : 
ZZ
fail
0

// 10. Invalid - no \r\n after chunk size
"POST /bad3 HTTP/1.1\r\nHost: err\r\nTransfer-Encoding: chunked\r\n\r\n4hello\r\n0\r\n\r\n"

ACTION : NONE

fd  = 10
========
Status               : P ERROR
Method               : POST
URI                  : /bhad3
Version              : HTTP/1.1
Header               : host
  Value              : err

Header               : transfer-encoding
  Value              : chunked

Body                 : 
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 13

Unparsed Data        : 
4hello
0


// 11. Invalid - incomplete chunk data
"POST /bad4 HTTP/1.1\r\nHost: bug\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhel\r\n0\r\n\r\n"

ACTION : NONE

fd  = 11
========
Status               : P ERROR
Method               : POST
URI                  : /bad4
Version              : HTTP/1.1
Header               : host
  Value              : bug

Header               : transfer-encoding
  Value              : chunked

Body                 : hel

ErrMess              : Malformed Body : Invalid chunk termination
ErrNum               : 400

Unparsed Length      : 13

Unparsed Data        : 
5
hel
0

// 12. Invalid - no Transfer-Encoding
"POST /bad5 HTTP/1.1\r\nHost: test\r\n\r\n4\r\ndata\r\n0\r\n\r\n"

ACTION : NONE


fd  = 12
========
Status               : P ERROR
Method               : POST
URI                  : /bad5
Version              : HTTP/1.1
Header               : host
  Value              : test

Body                 : 
ErrMess              : Malformed Request : Headers : content-length or transfer-encoding required for request type
ErrNum               : 400

Unparsed Length      : 14

Unparsed Data        : 
4
data
0


// 13. Valid - uppercase hex
"POST /upper HTTP/1.1\r\nHost: site.com\r\nTransfer-Encoding: chunked\r\n\r\nA\r\n0123456789\r\n0\r\n\r\n"

ACTION : NONE

fd  = 13
========
Status               : P DONE
Method               : POST
URI                  : /upper
Version              : HTTP/1.1
Header               : host
  Value              : site.com

Header               : transfer-encoding
  Value              : chunked

Body                 : 0123456789
ErrMess              : Parsing Complete
ErrNum               : 200

Unparsed Length      : 20

Unparsed Data        : 
A
0123456789
0


// 14. Valid - chunk size zero with trailer
"POST /trailer HTTP/1.1\r\nHost: host.com\r\nTransfer-Encoding: chunked\r\n\r\n0\r\nX-Debug: true\r\n\r\n"

ACTION : ADD CHUNKED TRAILER PARSING

fd  = 14
========
Status               : P ERROR
Method               : POST
URI                  : /trailer
Version              : HTTP/1.1
Header               : host
  Value              : host.com

Header               : transfer-encoding
  Value              : chunked

Body                 : 
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 20

Unparsed Data        : 
0
X-Debug: true

// 15. Valid - with CRLF inside data
"POST /crlf HTTP/1.1\r\nHost: data\r\nTransfer-Encoding: chunked\r\n\r\nB\r\nLine1\r\nLine2\r\n0\r\n\r\n"

ACTION : PARSING ERROR - valid \r\n in body content - this looks ok? Should be C for valid length

fd  = 15
========
Status               : P ERROR
Method               : POST
URI                  : /crlf
Version              : HTTP/1.1
Header               : host
  Value              : data

Header               : transfer-encoding
  Value              : chunked

Body                 : Line1
Line
ErrMess              : Malformed Body : Invalid chunk termination
ErrNum               : 400

Unparsed Length      : 22

Unparsed Data        : 
B
Line1
Line2
0

// 16. Valid - large chunk
"POST /big HTTP/1.1\r\nHost: x\r\nTransfer-Encoding: chunked\r\n\r\n10\r\n0123456789ABCDEF\r\n0\r\n\r\n"

ACTION : NONE

fd  = 16
========
Status               : P DONE
Method               : POST
URI                  : /big
Version              : HTTP/1.1
Header               : host
  Value              : x

Header               : transfer-encoding
  Value              : chunked

Body                 : 0123456789ABCDEF
ErrMess              : Parsing Complete
ErrNum               : 200

Unparsed Length      : 0

Unparsed Data        : 

  
// 17. Valid - minimal chunk + trailer
"POST /t HTTP/1.1\r\nHost: t.com\r\nTransfer-Encoding: chunked\r\n\r\n1\r\nx\r\n0\r\nT:1\r\n\r\n"

ACTION :  ADD TRAILER PARSING

fd  = 17
========
Status               : P ERROR
Method               : POST
URI                  : /t
Version              : HTTP/1.1
Header               : host
  Value              : t.com

Header               : transfer-encoding
  Value              : chunked

Body                 : x
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 16

Unparsed Data        : 
1
x
0
T:1

// 18. Invalid - negative chunk size
"POST /neg HTTP/1.1\r\nHost: bad\r\nTransfer-Encoding: chunked\r\n\r\n-5\r\nnope\r\n0\r\n\r\n"

ACTION : NONE

fd  = 18
========
Status               : P ERROR
Method               : POST
URI                  : /neg
Version              : HTTP/1.1
Header               : host
  Value              : bad

Header               : transfer-encoding
  Value              : chunked

Body                 : 
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 15

Unparsed Data        : 
-5
nope
0

// 19. Invalid - missing chunk data
"POST /nodata HTTP/1.1\r\nHost: h\r\nTransfer-Encoding: chunked\r\n\r\n3\r\n\r\n0\r\n\r\n"

ACTION : NONE

fd  = 19
========
Status               : P ERROR
Method               : POST
URI                  : /nodata
Version              : HTTP/1.1
Header               : host
  Value              : h

Header               : transfer-encoding
  Value              : chunked

Body                 : 
0
ErrMess              : Read Size of Zero : Incomplete message
ErrNum               : 400

Unparsed Length      : 10

Unparsed Data        : 
3

0

// 20. Invalid - malformed trailer
"POST /badtrailer HTTP/1.1\r\nHost: h\r\nTransfer-Encoding: chunked\r\n\r\n3\r\nfoo\r\n0\r\nInvalidHeader\r\n\r\n"

ACTION : ADD TRAILER PARSING

fd  = 20
========
Status               : P ERROR
Method               : POST
URI                  : /badtrailer
Version              : HTTP/1.1
Header               : host
  Value              : h

Header               : transfer-encoding
  Value              : chunked

Body                 : foo
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 2

Unparsed Data        : 

// 21. Invalid - space in chunk size
"POST /space HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\n\r\n3 \r\nabc\r\n0\r\n\r\n"

ACTION : NONE

fd  = 21
========
Status               : P ERROR
Method               : POST
URI                  : /sptace
Version              : HTTP/1.1
Header               : host
  Value              : host

Header               : transfer-encoding
  Value              : chunked

Body                 : 
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 14

Unparsed Data        : 
3 
abc
0

// 22. Invalid - junk after final zero
"POST /junk HTTP/1.1\r\nHost: host\r\nTransfer-Encoding: chunked\r\n\r\n1\r\nx\r\n0\r\n\r\nGARBAGE"

ACTION : ADD NEXT MESSAGE PARSING OR CREATE A STATUS TO INFORM SERVER OF ADDITIONAL DATA

fd  = 22
========
Status               : P ERROR
Method               : POST
URI                  : /junk
Version              : HTTP/1.1
Header               : host
  Value              : host

Header               : transfer-encoding
  Value              : chunked

Body                 : x
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 18

Unparsed Data        : 
1
x
0

GARBAGE

// 23. Valid - case-insensitive TE
"POST /cased HTTP/1.1\r\nHost: site\r\nTransfer-Encoding: Chunked\r\n\r\n2\r\nhi\r\n0\r\n\r\n"

ACTION : CHECK CASE SENSITIVITY

fd  = 23
========
Status               : P ERROR
Method               : POST
URI                  : /cased
Version              : HTTP/1.1
Header               : host
  Value              : site

Header               : transfer-encoding
  Value              : Chunked

Body                 : 
ErrMess              : Unsupported Transfer-Encoding: Chunked
ErrNum               : 501

Unparsed Length      : 12

Unparsed Data        : 
2
hi
0
// 24. Valid - chunked with query string
"POST /q?x=1 HTTP/1.1\r\nHost: s\r\nTransfer-Encoding: chunked\r\n\r\n6\r\nquery!\r\n0\r\n\r\n"

ACTION : NONE

fd  = 24
========
Status               : P DONE
Method               : POST
URI                  : /q?x=1
Version              : HTTP/1.1
Header               : host
  Value              : s

Header               : transfer-encoding
  Value              : chunked

Body                 : query!
ErrMess              : Parsing Complete
ErrNum               : 200

Unparsed Length      : 16

Unparsed Data        : 
6
query!
0


// 25. Valid - chunk with trailing LF
"POST /lf HTTP/1.1\r\nHost: h\r\nTransfer-Encoding: chunked\r\n\r\n2\r\nok\n\r\n0\r\n\r\n"

ACTION : CHECK THE VALIDITY OF THIS TRAILING LF "POST /lf HTTP/1.1\r\nHost: h\r\nTransfer-Encoding: chunked\r\n\r\n2\r\nok\n\r\n0\r\n\r\n" : Currently parsed as Malformed Body : Invalid chunk termination

fd  = 25
========
Status               : P ERROR
Method               : POST
URI                  : /lf
Version              : HTTP/1.1
Header               : host
  Value              : h

Header               : transfer-encoding
  Value              : chunked

Body                 : ok
ErrMess              : Malformed Body : Invalid chunk termination
ErrNum               : 400

Unparsed Length      : 8

Unparsed Data        : 


0

// 26. Valid - UTF-8 body
"POST /utf8 HTTP/1.1\r\nHost: h\r\nTransfer-Encoding: chunked\r\n\r\n4\r\nåßçø\r\n0\r\n\r\n"

ACTION : ADD TRAILER PARSING

fd  = 26
========
Status               : P ERROR
Method               : POST
URI                  : /utg9f8
Version              : HTTP/1.1
Header               : host
  Value              : h

Header               : transfer-encoding
  Value              : chunked

Body                 : \xc3\xa5\xc3\x9f
ErrMess              : Malformed Body : Invalid chunk termination
ErrNum               : 400

Unparsed Length      : 18

Unparsed Data        : 
4
åßçø
0

// 27. Valid - with custom trailer
"POST /custom HTTP/1.1\r\nHost: test\r\nTransfer-Encoding: chunked\r\n\r\n4\r\ntest\r\n0\r\nX-Trailer: yes\r\n\r\n"

ACTION : ADD TRAILER PARSING

fd  = 27
========
Status               : P ERROR
Method               : POST
URI                  : /custom
Version              : HTTP/1.1
Header               : host
  Value              : test

Header               : transfer-encoding
  Value              : chunked

Body                 : test
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 2

Unparsed Data        : 

// 28. Invalid - final CRLF missing
"POST /misscrlf HTTP/1.1\r\nHost: x\r\nTransfer-Encoding: chunked\r\n\r\n1\r\na\r\n0\r\n"

ACTION : INCORRECT ERROR : Malformed Body : Invalid Characters at end of chunking () : Should really be "Incomplete message"

fd  = 28
========
Status               : P ERROR
Method               : POST
URI                  : /misscrlf
Version              : HTTP/1.1
Header               : host
  Value              : x

Header               : transfer-encoding
  Value              : chunked

Body                 : a
ErrMess              : Read Size of Zero : Incomplete message
ErrNum               : 400

Unparsed Length      : 9

Unparsed Data        : 
1
a
0

// 29. Invalid - early EOF
"POST /eof HTTP/1.1\r\nHost: x\r\nTransfer-Encoding: chunked\r\n\r\n5\r\nhello\r\n"  // no 0 chunk

ACTION : NONE

fd  = 29
========
Status               : P ERROR
Method               : POST
URI                  : /eof
Version              : HTTP/1.1
Header               : host
  Value              : x

Header               : transfer-encoding
  Value              : chunked

Body                 : hello
ErrMess              : Read Size of Zero : Incomplete message
ErrNum               : 400

Unparsed Length      : 0

Unparsed Data        : 

// 30. Invalid - invalid hex with extra chars
"POST /extra HTTP/1.1\r\nHost: y\r\nTransfer-Encoding: chunked\r\n\r\n5z\r\nwrong\r\n0\r\n\r\n"

ACTION : NONE

fd  = 30
========
Status               : P ERROR
Method               : POST
URI                  : /extra
Version              : HTTP/1.1
Header               : host
  Value              : y

Header               : transfer-encoding
  Value              : chunked

Body                 : 
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 16

Unparsed Data        : 
5z
wrong
0

// 31. Invalid - headers after body start
"POST /oops HTTP/1.1\r\nHost: z\r\nTransfer-Encoding: chunked\r\nContent-Length: 10\r\n"  // Conflict\r\n2\r\nhi\r\n0\r\n\r\n"

ACTION : NONE

fd  = 31
========
Status               : P ERROR
Method               : POST
URI                  : /oops
Version              : HTTP/1.1
Header               : content-length
  Value              : 10

Header               : host
  Value              : z

Header               : transfer-encoding
  Value              : chunked

Body                 : 
ErrMess              : Malformed Request : Headers : Transfer-encoding and Content-Length
ErrNum               : 400

Unparsed Length      : 10

Unparsed Data        : 
2
hi
0

// 32. Invalid - trailing headers without 0
"POST /trail HTTP/1.1\r\nHost: site\r\nTransfer-Encoding: chunked\r\n\r\n2\r\nok\r\nX-Bad: oops\r\n"

ACTION : NONE

fd  = 32
========
Status               : P ERROR
Method               : POST
URI                  : /trail
Version              : HTTP/1.1
Header               : host
  Value              : site

Header               : transfer-encoding
  Value              : chunked

Body                 : ok
ErrMess              : Malformed Body : Invalid chunk size
ErrNum               : 400

Unparsed Length      : 20

Unparsed Data        : 
2
ok
X-Bad: oops