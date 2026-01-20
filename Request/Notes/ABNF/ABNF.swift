===========
Core Rules
===========

   Certain basic rules are in uppercase, such as SP, HTAB, CRLF, DIGIT,
   ALPHA, etc.

         ALPHA          =  %x41-5A / %x61-7A   ; A-Z / a-z

         BIT            =  "0" / "1"

         CHAR           =  %x01-7F
                                ; any 7-bit US-ASCII character,
                                ;  excluding NUL

         CR             =  %x0D
                                ; carriage return

         CRLF           =  CR LF
                                ; Internet standard newline

         CTL            =  %x00-1F / %x7F
                                ; controls

         DIGIT          =  %x30-39
                                ; 0-9

         DQUOTE         =  %x22
                                ; " (Double Quote) 											"

         HEXDIG         =  DIGIT / "A" / "B" / "C" / "D" / "E" / "F"

         HTAB           =  %x09
                                ; horizontal tab

         LF             =  %x0A
                                ; linefeed

         LWSP           =  *(WSP / CRLF WSP)
                                ; Use of this linear-white-space rule
                                ;  permits lines containing only white
                                ;  space that are no longer legal in
                                ;  mail headers and have caused
                                ;  interoperability problems in other
                                ;  contexts.
                                ; Do not use when defining mail
                                ;  headers and use with caution in
                                ;  other contexts.

         OCTET          =  %x00-FF
                                ; 8 bits of data

         SP             =  %x20

         VCHAR          =  %x21-7E
                                ; visible (printing) characters

         WSP            =  SP / HTAB
                                ; white space

	SPACE
		 BWS = OWS  			; BAD WHITESPACE - USED TO BE ALLOWED BUT NOW MUST BE REMOVED
		 OWS = *( SP / HTAB )	: OPTIONAL WHITESPACE
		 RWS = 1*( SP / HTAB )	: REQUIRED WHITESPACE


comment = "(" *( ctext / quoted-pair / comment ) ")"
	ctext = HTAB / SP / %x21-27 ; '!'-'''  / %x2A-5B ; '*'-'['  / %x5D-7E ; ']'-'~'  / obs-text
	//quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )

ctext = HTAB / SP / %x21-27 ; '!'-'''  / %x2A-5B ; '*'-'['  / %x5D-7E ; ']'-'~'  / obs-text

		obs-text = %x80-FF : OBSCURED TEST

qdtext = HTAB / SP / "!" / %x23-5B ; '#'-'[' / %x5D-7E ; ']'-'~' / obs-text
//quoted-pair = "\" ( HTAB / SP / VCHAR / obs-text )
quoted-string = DQUOTE *( qdtext / quoted-pair ) DQUOTE

tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "." / "^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA

token = 1*tchar
token68 = 1*( ALPHA / DIGIT / "-" / "." / "_" / "~" / "+" / "/" ) *"="

type = token

VCHAR = %x21-7E

============
Request Line
============

// METHOD
method = token

// URI

// URI STRUCTURE FOR HTTP/S
http-URI = "http://" authority path-abempty [ "?" query ]
https-URI = "https://" authority path-abempty [ "?" query ]

// URI AUTHORITY (user@host:port)
authority = <authority, see [URI], Section 3.2>
auth-scheme = token

uri-host = <host, see [URI], Section 3.2.2>
Host = uri-host [ ":" port ]
port = <port, see [URI], Section 3.2.3>


//PATH
relative-part = <relative-part, see [URI], Section 4.2>
absolute-URI = <absolute-URI, see [URI], Section 4.3>
absolute-path = 1*( "/" segment )

partial-URI = relative-part [ "?" query ]
path-abempty = <path-abempty, see [URI], Section 3.3>

// QUERY
query = <query, see [URI], Section 3.4>


segment = <segment, see [URI], Section 3.3>

URI-reference = <URI-reference, see [URI], Section 4.1>

version
=======
protocol = protocol-name [ "/" protocol-version ]
	protocol-name = token
	protocol-version = token

received-protocol = [ protocol-name "/" ] protocol-version

Header-Syntax
==============

field-content = field-vchar [ 1*( SP / HTAB / field-vchar ) field-vchar ]
field-name = token
field-value = *field-content
field-vchar = VCHAR / obs-text

parameter = parameter-name "=" parameter-value
parameter-name = token
parameter-value = ( token / quoted-string )
parameters = *( OWS ";" OWS [ parameter ] )

weight = OWS ";" OWS "q=" qvalue
qvalue = ( "0" [ "." *3DIGIT ] ) / ( "1" [ "." *3"0" ] )

Trailer = [ field-name *( OWS "," OWS field-name ) ]

Headers
=======

Accept = [ ( media-range [ weight ] ) *( OWS "," OWS ( media-range [ weight ] ) ) ]
	media-range = ( "*/*" / ( type "/*" ) / ( type "/" subtype ) ) parameters
	media-type = type "/" subtype parameters

Accept-Charset = [ ( ( token / "*" ) [ weight ] ) *( OWS "," OWS ( ( token / "*" ) [ weight ] ) ) ]
Accept-Encoding = [ ( codings [ weight ] ) *( OWS "," OWS ( codings [ weight ] ) ) ]
	codings = content-coding / "identity" / "*"
	content-coding = token

Accept-Language = [ ( language-range [ weight ] ) *( OWS "," OWS (language-range [ weight ] ) ) ]
	language-range = <language-range, see [RFC4647], Section 2.1>
	language-tag = <Language-Tag, see [RFC5646], Section 2.1>

Accept-Ranges = acceptable-ranges
	acceptable-ranges = range-unit *( OWS "," OWS range-unit )
	range-unit = token

// AUTHENTICATION
auth-param = token BWS "=" BWS ( token / quoted-string )

Allow = [ method *( OWS "," OWS method ) ]
Authentication-Info = [ auth-param *( OWS "," OWS auth-param ) ]
Authorization = credentials

challenge = auth-scheme [ 1*SP ( token68 / [ auth-param *( OWS "," OWS auth-param ) ] ) ]

Connection = [ connection-option *( OWS "," OWS connection-option ) ]
connection-option = token

Content-Encoding = [ content-coding *( OWS "," OWS content-coding ) ]
Content-Language = [ language-tag *( OWS "," OWS language-tag ) ]
Content-Length = 1*DIGIT
Content-Location = absolute-URI / partial-URI
Content-Range = range-unit SP ( range-resp / unsatisfied-range )
Content-Type = media-type

credentials = auth-scheme [ 1*SP ( token68 / [ auth-param *( OWS "," OWS auth-param ) ] ) ]

entity-tag = [ weak ] opaque-tag

etagc = "!" / %x23-7E ; '#'-'~' / obs-text

expectation = token [ "=" ( token / quoted-string ) parameters ]

ETag = entity-tag
Expect = [ expectation *( OWS "," OWS expectation ) ]

first-pos = 1*DIGIT

From = mailbox

If-Match = "*" / [ entity-tag *( OWS "," OWS entity-tag ) ]
If-Modified-Since = HTTP-date
If-None-Match = "*" / [ entity-tag *( OWS "," OWS entity-tag ) ]
If-Range = entity-tag / HTTP-date
If-Unmodified-Since = HTTP-date



last-pos = 1*DIGIT

Last-Modified = HTTP-date
Location = URI-reference

mailbox = <mailbox, see [RFC5322], Section 3.4>

Max-Forwards = 1*DIGIT


opaque-tag = DQUOTE *etagc DQUOTE
other-range = 1*( %x21-2B ; '!'-'+' / %x2D-7E ; '-'-'~' )

product = token [ "/" product-version ]
product-version = token

pseudonym = token

//PROXY
Proxy-Authenticate = [ challenge *( OWS "," OWS challenge ) ]
Proxy-Authentication-Info = [ auth-param *( OWS "," OWS auth-param ) ]
Proxy-Authorization = credentials

//RANGES
Range = ranges-specifier



range-resp = incl-range "/" ( complete-length / "*" )
	incl-range = first-pos "-" last-pos
	int-range = first-pos "-" [ last-pos ]
	complete-length = 1*DIGIT

range-set = range-spec *( OWS "," OWS range-spec )
	range-spec = int-range / suffix-range / other-range

ranges-specifier = range-unit "=" range-set
unsatisfied-range = "*/" complete-length
	complete-length = 1*DIGIT

received-by = pseudonym [ ":" port ]


Referer = absolute-URI / partial-URI
Retry-After = HTTP-date / delay-seconds

Server = product *( RWS ( product / comment ) )

subtype = token
suffix-length = 1*DIGIT
suffix-range = "-" suffix-length

transfer-coding = token *( OWS ";" OWS transfer-parameter )
transfer-parameter = token BWS "=" BWS ( token / quoted-string )
TE = [ t-codings *( OWS "," OWS t-codings ) ]
t-codings = "trailers" / ( transfer-coding [ weight ] )



Upgrade = [ protocol *( OWS "," OWS protocol ) ]
User-Agent = product *( RWS ( product / comment ) )

Vary = [ ( "*" / field-name ) *( OWS "," OWS ( "*" / field-name ) ) ]
Via = [ ( received-protocol RWS received-by [ RWS comment ] ) *( OWS "," OWS ( received-protocol RWS received-by [ RWS comment ] ) ) ]

weak = %x57.2F ; W/

WWW-Authenticate = [ challenge *( OWS "," OWS challenge ) ]



============
DATE && TIME
============

asctime-date = day-name SP date3 SP time-of-day SP year

Date = HTTP-date

date1 = day SP month SP year
date2 = day "-" month "-" 2DIGIT
date3 = month SP ( 2DIGIT / ( SP DIGIT ) )

delay-seconds = 1*DIGIT
day = 2DIGIT

day-name = %x4D.6F.6E ; Mon
 / %x54.75.65 ; Tue
 / %x57.65.64 ; Wed
 / %x54.68.75 ; Thu
 / %x46.72.69 ; Fri
 / %x53.61.74 ; Sat
 / %x53.75.6E ; Sun
day-name-l = %x4D.6F.6E.64.61.79 ; Monday
 / %x54.75.65.73.64.61.79 ; Tuesday
 / %x57.65.64.6E.65.73.64.61.79 ; Wednesday
 / %x54.68.75.72.73.64.61.79 ; Thursday
 / %x46.72.69.64.61.79 ; Friday
 / %x53.61.74.75.72.64.61.79 ; Saturday
 / %x53.75.6E.64.61.79 ; Sunday

GMT = %x47.4D.54 ; GMT

hour = 2DIGIT

HTTP-date = IMF-fixdate / obs-date

IMF-fixdate = day-name "," SP date1 SP time-of-day SP GMT

minute = 2DIGIT
month = %x4A.61.6E ; Jan
 / %x46.65.62 ; Feb
 / %x4D.61.72 ; Mar
 / %x41.70.72 ; Apr
 / %x4D.61.79 ; May
 / %x4A.75.6E ; Jun
 / %x4A.75.6C ; Jul
 / %x41.75.67 ; Aug
 / %x53.65.70 ; Sep
 / %x4F.63.74 ; Oct
 / %x4E.6F.76 ; Nov
 / %x44.65.63 ; Dec

 obs-date = rfc850-date / asctime-date

rfc850-date = day-name-l "," SP date2 SP time-of-day SP GMT

second = 2DIGIT

time-of-day = hour ":" minute ":" second

year = 4DIGIT
