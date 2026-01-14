# CONFIG CLASS

## CONFIG CLASS STRUCTURE

The Config class returns an object that contains information on the server structures provided in the config file

### CONFIG FILE

#### CONFIG FILE PATH

The path to the config file is either :

1. the path provided as a command line argument

2. the default_config defined by **DEFAULT_CONFIG** in the config class
    → only used when the program is started without command line argument

#### CONFIG FILE FORMAT

The config file is expected to be in JSON format.

## LOGGING

The Config object will be available to the entire program, so it makes sense to define the paths to the logs in the config class

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

Avoid logging inside tight loops

Avoid large strings

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

### SERVERS STRUCT AND VARIABLES

```c++
typedef struct server_s {

std::map<int, std::string>	error_pages;
std::vector<location_t> 	locations;			// Vector of location structs
std::string 				name;				// Server name
int 						port;				// Port that server will listen on
std::string 				root_dir;			// Root directory for server
std::vector<std::string>	server_methods;		// Methods permitted for all locations on server (can be superceded by location_methods)
} server_t;

```

#### LOCATIONS STRUCT AND VARIABLES

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