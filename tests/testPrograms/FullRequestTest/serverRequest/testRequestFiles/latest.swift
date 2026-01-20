// 3. Valid GET request
"GET /index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n"

ACTION : NONE

P_DONE : fd  = 3
Status               : P DONE
Method               : GET
URI                  : /index.html
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

// 4. Valid POST request
"POST /submit-form HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nname=John+Doe&age=30",

ACTION : VALID REQUEST : add support for x-www-form-urlencoded

P_ERROR : fd  = 4
Status               : P ERROR
Method               : POST
URI                  : /submit-form
Version              : HTTP/1.1
Header               : content-length
  Value              : 27

Header               : content-type
  Value              : application/x-www-form-urlencoded

Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : Content-Type not supported
ErrNum               : 415

// 5. Valid DELETE request
"DELETE /resource/123 HTTP/1.1\r\nHost: api.example.com\r\n\r\n",

ACTION : NONE

P_ERROR : fd  = 5
Status               : P DONE
Method               : DELETE
URI                  : /resource/123
Version              : HTTP/1.1
Header               : host
  Value              : api.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200


// 6. Invalid request: Missing HTTP version
"GET /home\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

P_ERROR : fd  = 6
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too few parameters in request line
ErrNum               : 400

// 7. Invalid request: Unsupported HTTP method
"PUT /update HTTP/1.1\r\nHost: www.example.com\r\n\r\n",

ACTION : NEED TO VALIDATE METHOD //VALID REQUEST? : NEED TO HANDLE PUT? 

P_ERROR : fd  = 7
Status               : P ERROR
Method               : PUT
URI                  : /update
Version              : HTTP/1.1
Body                 : 
ErrMess              : Method Not Supported
ErrNum               : 501

// 8. Invalid request: Malformed request line
"GET /index.html HTTP/1.1 EXTRA\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

P_ERROR : fd  = 8
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too many parameters in request line
ErrNum               : 400

// 9. Valid GET with query string
"GET /search?q=chatgpt&lang=en HTTP/1.1\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

P_DONE : fd  = 9
Status               : P DONE
Method               : GET
URI                  : /search?q=chatgpt&lang=en
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

// 10. Invalid POST: Missing Content-Length header
"POST /submit HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/json\r\n\r\n{\"name\":\"John\"}",

ACTION REQUIRED : NONE

fd  = 10
========
Status               : P ERROR
Method               : POST
URI                  : /submit
Version              : HTTP/1.1
Header               : content-type
  Value              : application/json

Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : Malformed Request : Headers : content-length or transfer-encoding required for request type
ErrNum               : 400

// 11. Valid POST with JSON
"POST /api/users HTTP/1.1\r\nHost: api.example.com\r\nContent-Type: application/json\r\nContent-Length: 20\r\n\r\n{\"username\":\"alice\"}",

ACTION : NONE

P_ERROR : fd  = 11
Status               : P DONE
Method               : POST
URI                  : /api/users
Version              : HTTP/1.1
Header               : content-length
  Value              : 20

Header               : content-type
  Value              : application/json

Header               : host
  Value              : api.example.com

Body                 : {"username":"alice"}
ErrMess              : Parsing Complete
ErrNum               : 200

// 12. Invalid GET: No Host header (HTTP/1.1 requires it)
"GET /about HTTP/1.1\r\n\r\n",

ACTION : NONE

P_ERROR : fd  = 12
Status               : P ERROR
Method               : GET
URI                  : /about
Version              : HTTP/1.1
Body                 : 
ErrMess              : Missing 'Host' header
ErrNum               : 400

// 13. Valid DELETE with query params
"DELETE /items?id=42 HTTP/1.1\r\nHost: api.example.com\r\n\r\n",

ACTION : None

P_DONE : fd  = 13
Status               : P DONE
Method               : DELETE
URI                  : /items?id=42
Version              : HTTP/1.1
Header               : host
  Value              : api.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200


// 14. Invalid request: Bad HTTP version
"GET /home HTTP/2.0\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

P_ERROR : fd  = 14
Status               : P ERROR
Method               : GET
URI                  : /home
Version              : HTTP/2.0
Body                 : 
ErrMess              : Unsupported Version
ErrNum               : 505

// 15. Valid GET: root request
"GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

P_DONE : fd  = 15
Status               : P DONE
Method               : GET
URI                  : /
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200


// 16. Invalid request: Method in lowercase
"get /page HTTP/1.1\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

P_ERROR : fd  = 16
Status               : P ERROR
Method               : get
URI                  : /page
Version              : HTTP/1.1
Body                 : 
ErrMess              : Method must be uppercase
ErrNum               : 400

// 17. Valid POST: form submission
"POST /login HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 29\r\n\r\nusername=bob&password=secret",

ACTION : ADD SUPPORT FOR "application/x-www-form-urlencoded" 
         PARSE BODY

P_ERROR : fd  = 17
Status               : P ERROR
Method               : POST
URI                  : /login
Version              : HTTP/1.1
Header               : content-length
  Value              : 29

Header               : content-type
  Value              : application/x-www-form-urlencoded

Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : Content-Type not supported
ErrNum               : 415

// 18. Invalid request: Missing request URI
"GET  HTTP/1.1\r\nHost: www.example.com\r\n\r\n",

ACTION : MISSING URI : ADD CHECK FOR EMPTY TOKEN TO URI FUNCTION
          MUST HAVE AT LEAST / IF ORIGIN SERVER FORM

P_DONE : fd  = 18
Status               : P DONE
Method               : GET
URI                  : 
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

// 19. Valid DELETE: delete resource with slash
"DELETE /users/789 HTTP/1.1\r\nHost: api.example.com\r\n\r\n",

ACTION : FIX PATH

P_ERROR : fd  = 19
Status               : P DONE
Method               : DELETE
URI                  : /users/789
Version              : HTTP/1.1
Header               : host
  Value              : api.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

// 20. Invalid POST: Content-Length mismatch (actual body smaller)
"POST /submit HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: text/plain\r\nContent-Length: 10\r\n\r\nHello",

ACTION : FIX CONTENT LENGTH VALIDATION

P_ERROR : fd  = 20
Status               : P ERROR
Method               : POST
URI                  : /submit
Version              : HTTP/1.1
Header               : content-length
  Value              : 10

Header               : content-type
  Value              : text/plain

Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : Content-Type not supported
ErrNum               : 415

// 21. Valid GET with custom header
"GET /dashboard HTTP/1.1\r\nHost: www.example.com\r\nX-Custom-Header: 12345\r\n\r\n",

ACTION : NONE

P_DONE : fd  = 21
Status               : P DONE
Method               : GET
URI                  : /dashboard
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Header               : x-custom-header
  Value              : 12345

Body                 : 
ErrMess              : 
ErrNum               : 200

// 22. Invalid GET: Missing space between method and URI
"GET/index.html HTTP/1.1\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too few parameters in request line
ErrNum               : 400

// 23. Valid POST with multipart/form-data
"POST /upload HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: multipart/form-data; boundary=----WebKitFormBoundary\r\nContent-Length: 138\r\n\r\n------WebKitFormBoundary\r\nContent-Disposition: form-data; name=\"file\"; filename=\"test.txt\"\r\nContent-Type: text/plain\r\n\r\nHello world\r\n------WebKitFormBoundary--\r\n",

ACTION : ADD MULTIPART SUPPORT

P_ERROR : fd  = 23
Status               : P ERROR
Method               : POST
URI                  : /upload
Version              : HTTP/1.1
Header               : content-length
  Value              : 138

Header               : content-type
  Value              : multipart/form-data

Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : Missing multiPart boundary
ErrNum               : 400

// 24. Invalid DELETE: Extra characters after HTTP version
"DELETE /resource/456 HTTP/1.1 extra\r\nHost: api.example.com\r\n\r\n",

ACTION : NONE

P_ERROR : fd  = 24
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too many parameters in request line
ErrNum               : 400


// 25. Valid GET with fragment (fragment ignored in request)
"GET /page.html#section1 HTTP/1.1\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

P_DONE : fd  = 25
Status               : P DONE
Method               : GET
URI                  : /page.html#section1
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200


// 26. Invalid POST: Missing empty line before body
"POST /submit HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/json\r\nContent-Length: 18\r\n{\"name\":\"alice\"}"

ACTION : FIND OUT HOW TO DIFFERENTIATE HEADERS/BODY : VALID HEADERS CHECK?

Read operation = 0 : fd  = 26
P_ERROR : fd  = 26
Status               : P ERROR
Method               : POST
URI                  : /submit
Version              : HTTP/1.1
Header               : content-length
  Value              : 18

Header               : content-type
  Value              : application/json

Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : Read Size of Zero : Incomplete message
ErrNum               : 400

// 27. Valid GET: multiple headers
"GET /info HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: CustomClient/1.0\r\nAccept: text/html\r\n\r\n",

ACTION : NONE 

P_DONE : fd  = 27
Status               : P DONE
Method               : GET
URI                  : /info
Version              : HTTP/1.1
Header               : accept
  Value              : text/html

Header               : host
  Value              : www.example.com

Header               : user-agent
  Value              : CustomClient/1.0

Body                 : 
ErrMess              : 
ErrNum               : 200

// 28. Invalid GET: Spaces in URI without encoding
"GET /file name.txt HTTP/1.1\r\nHost: www.example.com\r\n\r\n",

ACTION : NONE

P_ERROR : fd  = 28
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too many parameters in request line
ErrNum               : 400

// 29. Valid POST: empty body (Content-Length: 0)
"POST /ping HTTP/1.1\r\nHost: www.example.com\r\nContent-Length: 0\r\n\r\n",

ACTION : NONE

P_DONE : fd  = 29
Status               : P DONE
Method               : POST
URI                  : /ping
Version              : HTTP/1.1
Header               : content-length
  Value              : 0

Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

// 30. Invalid DELETE: lowercase HTTP version
"DELETE /item/123 http/1.1\r\nHost: api.example.com\r\n\r\n",

ACTION : FIX PATH

P_ERROR : fd  = 30
Status               : P ERROR
Method               : DELETE
URI                  : /item/123
Version              : http/1.1
Body                 : 
ErrMess              : Unsupported Version
ErrNum               : 400

// 31. Valid GET with IPv6 host
"GET /status HTTP/1.1\r\nHost: [2001:db8::1]\r\n\r\n",

ACTION : HANDLE IPv6

P_DONE : fd  = 31
Status               : P DONE
Method               : GET
URI                  : /status
Version              : HTTP/1.1
Header               : host
  Value              : [2001:db8::1]

Body                 : 
ErrMess              : 
ErrNum               : 200



// 32. Invalid POST: Missing Host header and Content-Length
"POST /data HTTP/1.1\r\nContent-Type: application/json\r\n\r\n{\"data\":\"test\"}"

P_ERROR : fd  = 32
Status               : P ERROR
Method               : POST
URI                  : /data
Version              : HTTP/1.1
Header               : content-type
  Value              : application/json

Body                 : 
ErrMess              : Missing 'Host' header
ErrNum               : 400