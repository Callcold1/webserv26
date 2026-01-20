Read operation = 0 : fd  = 27
P_ERROR : fd  = 27
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

fd  = 3
========
Status               : P DONE
Method               : GET
URI                  : /index.html
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

fd  = 4
========
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

fd  = 5
========
Status               : P DONE
Method               : DELETE
URI                  : /resource/123
Version              : HTTP/1.1
Header               : host
  Value              : api.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

fd  = 6
========
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too few parameters in request line
ErrNum               : 400

fd  = 7
========
Status               : P ERROR
Method               : PUT
URI                  : /update
Version              : HTTP/1.1
Body                 : 
ErrMess              : Method Not Supported
ErrNum               : 501

fd  = 8
========
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too many parameters in request line
ErrNum               : 400

fd  = 9
========
Status               : P DONE
Method               : GET
URI                  : /search?q=chatgpt&lang=en
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

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

fd  = 11
========
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

fd  = 12
========
Status               : P ERROR
Method               : GET
URI                  : /about
Version              : HTTP/1.1
Body                 : 
ErrMess              : Missing 'Host' header
ErrNum               : 400

fd  = 13
========
Status               : P DONE
Method               : DELETE
URI                  : /items?id=42
Version              : HTTP/1.1
Header               : host
  Value              : api.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

fd  = 14
========
Status               : P ERROR
Method               : GET
URI                  : /home
Version              : HTTP/2.0
Body                 : 
ErrMess              : Unsupported Version
ErrNum               : 505

fd  = 15
========
Status               : P DONE
Method               : GET
URI                  : /
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

fd  = 16
========
Status               : P ERROR
Method               : get
URI                  : /page
Version              : HTTP/1.1
Body                 : 
ErrMess              : Method must be uppercase
ErrNum               : 400

fd  = 17
========
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

fd  = 18
========
Status               : P DONE
Method               : GET
URI                  : 
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

fd  = 19
========
Status               : P DONE
Method               : DELETE
URI                  : /users/789
Version              : HTTP/1.1
Header               : host
  Value              : api.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

fd  = 20
========
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

fd  = 21
========
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

fd  = 22
========
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too few parameters in request line
ErrNum               : 400

fd  = 24
========
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

fd  = 25
========
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too many parameters in request line
ErrNum               : 400

fd  = 26
========
Status               : P DONE
Method               : GET
URI                  : /page.html#section1
Version              : HTTP/1.1
Header               : host
  Value              : www.example.com

Body                 : 
ErrMess              : 
ErrNum               : 200

fd  = 27
========
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

fd  = 28
========
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

fd  = 29
========
Status               : P ERROR
Method               : 
URI                  : 
Version              : 
Body                 : 
ErrMess              : Malformed Request Line : too many parameters in request line
ErrNum               : 400

fd  = 30
========
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

fd  = 31
========
Status               : P ERROR
Method               : DELETE
URI                  : /item/123
Version              : http/1.1
Body                 : 
ErrMess              : Unsupported Version
ErrNum               : 400

fd  = 32
========
Status               : P DONE
Method               : GET
URI                  : /status
Version              : HTTP/1.1
Header               : host
  Value              : [2001:db8::1]

Body                 : 
ErrMess              : 
ErrNum               : 200

fd  = 33
========
Status               : P ERROR
Method               : POST
URI                  : /data
Version              : HTTP/1.1
Header               : content-type
  Value              : application/json

Body                 : 
ErrMess              : Missing 'Host' header
ErrNum               : 400

