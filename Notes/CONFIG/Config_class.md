# CONFIG/CONFIG PARSER

## CONFIG/CONFIG PARSER CLASS STRUCTURE

The ConfigParser class returns a pointer to a Config object.
It allocates memory for a Config object, and returns a pointer to that object.

The ConfigParser is called by means of a static function, and so no ConfigParser object is instantiated.

The calling function is responsible or deleting the Config object.

## CONFIG STRUCTURE

The initial section of the config file contains definitions of variables that apply globally.
This section is followed by a series of nested blocks of the following types :

    GLOBAL VARIABLES
          |
      HTTP BLOCK (contains any number of server blocks)
          |
          └── SERVER BLOCK (contains Server-level variables and any number of Location blocks)
                    |
                    └── LOCATION BLOCK(contains Location-level variables)

### CONFIG STRUCTURE TABLE

|   BLOCK |   DELIMITER   |   CONTENTS   |  ASSOCIATED CLASS   |
|---------|---------------|--------------|---------------------|
| HTTP | {} | Variables that apply at global-level | Config |
|        |    | Server Blocks | |
| SERVER | {} | Variables that apply at server-level | ConfigServer |
|        |    | Location Blocks | |
| LOCATION | {} | Variables that apply at location-level | ConfigLocation |

### CONFIG HIERARCHY

- Inner scopes **inherit** from outer scopes unless overridden
- Not every directive is valid in every scope
- Some directives are intentionally restricted for correctness and performance

The Config file is essentially parsed into 3 different object types:
|   Class |   Variable   | Parsed? |   Source   |  Description |
|---------|--------------|---------|------------|--------------|
| Config | _config_file | No | Command line arg or DEFAULT_CONFIG used (path is defined in Config.hpp) | Path to config |
| | _error_pages | Yes | HTTP block| Error codes (and corresponding paths to error-file?) |
| | _log_names | Yes | pre-HTTP block | Name of logs to be created and formats? |
| | _max_body | Yes | HTTP block | Maximum body size in bytes |
| | _max_connections | Yes | HTTP block | Maximum number of connections (across all servers - informs server/poll functions) |
| | _servers | Yes | HTTP block | Container of servers (one for each server block - should it be one for each port? Do we support virtual servers?) |
| | _timeout_ka | Opt | HTTP block or DEFAULT_TIMEOUT_RW in Config.hpp | Default timeout for keep-alive |
| | _timeout_rw | Opt | HTTP block | Default timeout for read/write operations |
| | | | |
| ConfigServer | Yes | Server block | |
| ConfigLocation | | | Location block |

## CONFIG CLASS

Config class objects contain the following variables :

```c++
std::string 						_config_file; 			// config file name	(if not provided, revert to DEFAULT_CONF)
std::map<int, std::string> 			_error_pages;			// Map of error pages to generate
std::vector<std::string>			_log_names;				// Vector of log names to be generated
long long							_max_body;				// Max acceptable body size in bytes
int									_max_connections; 		// (e.g. 1024)
std::vector<ConfigServer>			_servers;				// Vector of servers
struct tm							_timeout_ka;			// Length of time before keep-alive timeout
struct tm							_timeout_rw;			// Length of time before rw timeout
```

## SERVER CLASS

```c++
std::map<int, std::string>			_error_pages;
std::vector<ConfigLocation> 		_locations;			// Vector of location structs
std::vector<std::string>			_methods;			// Methods permitted for all locations on server (can be superceded by location_methods)
std::string 						_name;				// Server name
int 								_port;				// Port that server will listen on
std::string 						_root_dir;			// Root directory for server - overrides global value
```

## LOCATION CLASS

```c++
bool								_autoindex;			// Indicates if the location has autoindex ()
std::string							_index;
std::vector<std::string>			_location_methods;
std::string 						_name;
std::string							_path;
bool								_redirect_status;
std::string							_redirect_url;
std::string							_root_dir;			// Root directory for location
```
### CONFIG FILE

#### CONFIG FILE PATH

The path to the config file is either :

1. the path provided as a command line argument

2. the default_config defined by **DEFAULT_CONFIG** in the config class
    → only used when the program is started without command line argument

#### CONFIG FILE FORMAT

The config file is expected to be in JSON format (Global variables may precede the JSON format, which should start from point 'http {}' or 'HTTP {}')

## LOGGING

Separate logs will be created for

1. Startup/Shutdown Log

        • Server start time

        • Config file path used

        • Parsed servers / ports / hostnames

        • Root directories

        • Autoindex status per location

        • Max body size

        • CGI mappings

2. Connections Log

        • New client acceptance

        • Client socket fd

        • Client IP + port

        • Connection close reason

        • When fd added/removed from poll/epoll

        • POLLIN / POLLOUT transitions

3. Request Parsing (for debugging)

        • Raw request line

        • Parsed method, URI, version

        • Headers count

        • Body size

        • Chunked vs Content-Length

        • Parsing errors

4. HTTP parsing & validation Log

        • Method allowed or rejected

        • Location match

        • Redirection applied

        • Client max body exceeded

5. File system & resource access logs

        • Resolved filesystem path

        • stat/access failures

        • Directory vs file

        • Autoindex triggered

6. Response logs

        • Status code

        • Response size

        • Method + URI

        • Processing time (if possible)

7. CGI logs

        • CGI start / end

        • Executable path

        • Script path

        • Exit code

        • Timeout / kill

8. Error logs

        • ERROR → server misbehavior

        • WARN → client misbehavior

        • INFO → normal lifecycle

        • DEBUG → parsing/internal state

### FILE PATHS FOR LOGS

All logs will be saved to "webserv/logs/"

| Log | DEFINITION | Purpose |
| ----| ---------- | --------|
| log_conf | LOG_PATH_CONF | Startup/Shutdown/Config Info |
| log_conn | Connections Log | Connections Info |
| log_request_parsing | Request Parsing (for debugging) | Request Parsing Info |
| log_request_status | HTTP parsing & validation Log | Request Info |
| log_access | File system & resource access logs | File Access Info |
| log_response | Response logs | Response Info |
| log_cgi | CGI logs | CGI Info |
| log_errors | Error logs | Error Info |

### LOGGING LEVELS

Use an enum to describe the logging level :

    enum LogLevel {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL
    };

### LOGGING FORMAT

Logs will use the following format :

    [TIMESTAMP] [LEVEL] message

Access Log format

   [TIMESTAMP] client "request" status size time

### LOG CODE

Logger::log(LOG_ERROR, "poll() failed: " + strerror(errno));

### LOGGING OTHER

Thread/fork safe

Write using write() (not std::cout)

Open log file once

Use O_APPEND

Non-blocking safe

Logging must not block:

- Avoid logging inside tight loops

- Avoid large strings

## CONFIG CLASS ERROR HANDLING

An error will be thrown, and the program will exit if any of the following are true :

- the format of the server(s) isn't viable (i.e. lacking enough information to create a basic server)
- there is an error in the JSON format
- file paths provided do not exist
- file paths are inaccessible

## CONFIG CLASS VARIABLES

The variables held in the config class are as follows :



Some are mandatory and some are optional

### MANDATORY VARIABLES

### OPTIONAL VARIABLES

### SERVERS CLASS AND VARIABLES

```c++
class ConfigServer {

std::map<int, std::string>  _error_pages;
std::vector<ConfigLocation> _locations;			// Vector of location structs
std::vector<std::string>    _methods;			// Methods permitted for all locations on server (can be superceded by location_methods)
std::string                 _name;				// Server name
int                         _port;				// Port that server will listen on
std::string                 _root_dir;			// Root directory for server - overrides global value
};

```

#### LOCATION CLASS AND VARIABLES

##### AUTOINDEX

When a request targets a directory, the server logic is typically:

1. Resolve URI → filesystem path

2. If path is a file
    → serve file

3. If path is a directory

    • If an index file exists (index.html, index.php, etc.)
        → serve it

    • Else if autoindex ON
        → generate an HTML page listing directory contents

    • Else → return 403 Forbidden