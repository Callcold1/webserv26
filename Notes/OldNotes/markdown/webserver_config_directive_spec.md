# Webserver Configuration Directive Specification

## Single Source of Truth

------------------------------------------------------------------------

# 1. Core Design Principle

Hierarchy model:

    GLOBAL
     └── SERVER
           └── LOCATION

Each directive must define:

-   Allowed contexts (GLOBAL / SERVER / LOCATION)
-   Number of arguments
-   Duplicate behavior inside the same block
-   Inheritance behavior (override, merge, replace, none)
-   Whether it is required or optional
-   Whether only a single instance is allowed

------------------------------------------------------------------------

# 2. Directive Categories

## A. Single Argument Directives

Only one argument allowed. Parsing must fail if: - More than one
argument - Missing argument

Examples:

-   max_body
-   max_connections
-   max_header_size
-   timeout_ka
-   timeout_rw
-   root
-   alias
-   autoindex
-   cgi_enabled
-   upload_path
-   redirect_status
-   redirect_url
-   modifier

------------------------------------------------------------------------

## B. Multiple Argument Directives

Allow multiple arguments per directive line.

Examples:

-   server_name
-   methods
-   index
-   charset
-   listen
-   try_files
-   error_page (code + path)
-   cgi_map (extension + binary path)

------------------------------------------------------------------------

# 3. Duplicate Directive Behavior (Same Block)

## 1) Overwrite (Last Wins)

Recommended for: - Scalars - Booleans - Single pointer fields

Examples: - max_body - root - alias - upload_path - timeout\_\*

------------------------------------------------------------------------

## 2) Append / Accumulate

Recommended for: - vectors - maps - additive configuration

Examples: - methods - server_name - index - charset - listen -
error_page - try_files

------------------------------------------------------------------------

## 3) Hard Error (Only One Allowed)

Optional stricter rule for: - alias - redirect - root (optional)

For a 42 project, prefer: - Overwrite for scalars - Append for vectors -
Avoid hard errors unless logically required

------------------------------------------------------------------------

# 4. Inheritance Model

General Rule:

LOCATION overrides SERVER overrides GLOBAL.

------------------------------------------------------------------------

## A. Scalar Limits

-   Inherit downward
-   Child replaces parent
-   No merging

Applies to: - max_body - max_header_size - timeouts

------------------------------------------------------------------------

## B. Methods

Recommended model: - Location methods fully override server methods

------------------------------------------------------------------------

## C. error_page

-   Inherit and merge downward
-   Same error code overwrites parent mapping

------------------------------------------------------------------------

## D. charset

Simplest model: - If defined in child, replace parent entirely -
Otherwise inherit

------------------------------------------------------------------------

## E. root / alias

Rules:

-   If alias is defined → ignore root
-   Location root overrides server root
-   Server root overrides global root
-   No merging

------------------------------------------------------------------------

## F. index

-   If location defines index → use only location index
-   Otherwise inherit

------------------------------------------------------------------------

## G. try_files

-   Only meaningful in LOCATION
-   No inheritance

------------------------------------------------------------------------

## H. cgi_map

-   Merge downward
-   Child overrides same extension

------------------------------------------------------------------------

# 5. Master Directive Tables

------------------------------------------------------------------------

## GLOBAL

  ----------------------------------------------------------------------------------
  Directive         Args    Multi Allowed  Duplicate Behavior   Inherited   Notes
  ----------------- ------- -------------- -------------------- ----------- --------
  charset           multi   yes            Append               Replace     

  error_page        multi   yes            Append (by code)     Merge       

  max_body          1       no             Overwrite            Replace     Scalar

  max_connections   1       no             Overwrite            Replace     

  max_header_size   1       no             Overwrite            Replace     

  timeout_ka        1       no             Overwrite            Replace     

  timeout_rw        1       no             Overwrite            Replace     
  ----------------------------------------------------------------------------------

------------------------------------------------------------------------

## SERVER

  --------------------------------------------------------------------------------------------
  Directive         Args    Multi     Duplicate     Inherited from  Inherited to    Notes
                            Allowed   Behavior      Global          Location        
  ----------------- ------- --------- ------------- --------------- --------------- ----------
  charset           multi   yes       Append        Replace         Replace         

  error_page        multi   yes       Append        Merge           Merge           

  max_body          1       no        Overwrite     Replace         Replace         

  max_connections   1       no        Overwrite     Replace         No              

  max_header_size   1       no        Overwrite     Replace         Replace         

  methods           multi   yes       Append        Replace         Replace         

  listen            multi   yes       Append        No              No              Required

  root              1       no        Overwrite     Replace         Replace         

  server_name       multi   yes       Append        No              No              

  locations         block   yes       Append        No              No              
  --------------------------------------------------------------------------------------------

------------------------------------------------------------------------

## LOCATION

  ---------------------------------------------------------------------------------------
  Directive         Args    Multi Allowed  Duplicate Behavior   Inherited   Notes
  ----------------- ------- -------------- -------------------- ----------- -------------
  alias             1       no             Overwrite            No          Overrides
                                                                            root

  autoindex         1       no             Overwrite            Replace     bool

  cgi_enabled       1       no             Overwrite            Replace     bool

  cgi_map           multi   yes            Append (by ext)      Merge       

  charset           multi   yes            Append               Replace     

  index             multi   yes            Append               Replace     

  methods           multi   yes            Append               Replace     

  modifier          1       no             Overwrite            No          Use enum

  redirect_status   1       no             Overwrite            No          Combine with
                                                                            URL

  redirect_url      1       no             Overwrite            No          Combine with
                                                                            status

  root              1       no             Overwrite            Replace     

  try_files         multi   yes            Overwrite            No          No
                                                                            inheritance

  upload_path       1       no             Overwrite            Replace     

  uri               1       no             Overwrite            No          Likely
                                                                            unnecessary
  ---------------------------------------------------------------------------------------

------------------------------------------------------------------------

# 6. Design Critique & Improvements

## 1. Combine redirect_status + redirect_url

Better model:

    struct Redirect {
        int status;
        std::string url;
    };

Use a single directive like:

    return 301 /newpath;

------------------------------------------------------------------------

## 2. Remove cgi_enabled

If cgi_map is empty → CGI disabled. Boolean is redundant.

------------------------------------------------------------------------

## 3. Replace modifier with Enum

    enum LocationMatchType {
        EXACT,
        PREFIX,
        REGEX
    };

------------------------------------------------------------------------

## 4. Avoid Raw fileInfo_s\* in Config

Config layer should store paths as strings. Filesystem resolution should
happen later.

------------------------------------------------------------------------

## 5. max_connections

Logically global. Consider removing from SERVER unless doing per-vhost
limits.

------------------------------------------------------------------------

## 6. \_uri in Location

Redundant. Location name already represents URI.

------------------------------------------------------------------------

# 7. Recommended Clean Model

GLOBAL: - limits - default charset - default error pages

SERVER: - listen - server_name - root - limits override - methods
default - error_page override

LOCATION: - match type - path - root/alias - methods override - index
override - try_files - autoindex - cgi_map - upload_path - redirect

------------------------------------------------------------------------

# 8. Final Parsing Rules

## Within Same Block

  Type                       Behavior
  -------------------------- ------------------
  Scalar                     Last wins
  Vector                     Append
  Map                        Overwrite by key
  Unique logical directive   Optional error

------------------------------------------------------------------------

## Across Nested Blocks

  Type                             Behavior
  -------------------------------- --------------------------------
  Scalar                           Child replaces parent
  Vector (methods/index/charset)   Child replaces parent entirely
  error_page                       Merge by error code
  cgi_map                          Merge by extension
  try_files                        No inheritance
  redirect                         No inheritance

------------------------------------------------------------------------

# 9. Recommended Parsing Architecture

1.  Parse blocks into raw directive storage
2.  Store without resolving inheritance
3.  After parsing, run normalization pass
4.  Resolve inheritance
5.  Validate final resolved configuration

Never resolve inheritance during parsing.
