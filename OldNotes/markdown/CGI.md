# 4.4.  The Script Command Line

   Some systems support a method for supplying an array of strings to
   the CGI script.  This is only used in the case of an 'indexed' HTTP
   query, which is identified by a 'GET' or 'HEAD' request with a URI
   query string that does not contain any unencoded "=" characters.  For
   such a request, the server SHOULD treat the query-string as a
   search-string and parse it into words, using the rules

      search-string = search-word *( "+" search-word )
      search-word   = 1*schar
      schar         = unreserved | escaped | xreserved
      xreserved     = ";" | "/" | "?" | ":" | "@" | "&" | "=" | "," |
                      "$"

   After parsing, each search-word is URL-decoded, optionally encoded in
   a system-defined manner and then added to the command line argument
   list.

   If the server cannot create any part of the argument list, then the
   server MUST NOT generate any command line information.  For example,
   the number of arguments may be greater than operating system or
   server limits, or one of the words may not be representable as an
   argument.

   The script SHOULD check to see if the QUERY_STRING value contains an
   unencoded "=" character, and SHOULD NOT use the command line
   arguments if it does.
