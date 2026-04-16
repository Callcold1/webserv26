Request Handler
===============

So the segfault/problems with the path is due to "path = _re.paths.resource_path;" being used at the start of the function - see below.

I think that rather than setting the path at the start of the request_handler,
it might make more sense to set the path in the relevant response classes.

_request->_re.paths : Different paths are populated depending on the Response type required.

                      The path used depends on the response type (Determined whilst parsing request) 
                      The response type is already set in _re.type.

_request->_re.type  : Type of response required 
                      
                      RESPOND_ERROR   : use paths.error_path
                                        (populated with any error paths from Config)

                      RESPOND_NOT_SET : use paths.error.path
                      
                      RESPOND_REDIR   : use paths.redirect_url
                      
                      RESPOND_CGI     : paths.resource_path.path    : path of script to execute
                                        paths.resource_path.is_exec : true if executable
                      
                      RESPOND_FILE    : use paths.resource.path.path

                      RESPOND_MP_UP   Any multipart parts that are files are already saved to files in /tmp
                                      
                                      : use paths.upload_paths : this is a vector of pairs

                                      : paths.upload_paths[i]->first.path : path that temp file should be moved to
                                      : paths.upload_paths[i]->second     : temp file path                                        
                                            

                      RESPOND_UPLOAD  : use paths.upload_paths

                      RESPOND_INDEX   : handled already in SuccessResponse so I'll delete this unless it helps

The request_handler also contains a lot of if statements to determine which type of response is required,
but that has already been done - it would be easier to use _re.type to determine which response type to create.

I know you added additional logic to the CGI section, but I'm not sure this is required - see CGI below....

CGI
===
I think the CGI isn't set up as it should be.
Using a bool to determine if it's python or php doesn't feel like the correct way to do things.

We have a directive in the config file (cgi_path).

Any location that is used for CGI needs to have the CGI path set.
There can be multiple instances of this in a single location.

                                       : e.g. cgi_path  /www/cgi-bin .php;
                                              cgi_path  /www/cgi-bin .py;

From what I've read, the type of script can be determined by the kernel if it contains a shebang.
So as long as the script is executable and the program path exist (i.e. #!/program_path) it will run.
Installing dependencies should be handled from the script.

So if the cgi_path is set in the config :  
    file ext in config     : if executable, treat as CGI (i.e. execute)
    file ext not in config : we treat as a static file request.

If the shebang doesn't match the file ext, we can either ignore it and execute anyway (I think this is the normal approach),

I know selecting the interpreter works too, but I think it's cleaner just to execute any scripts with a valid name.
If the script is misconfigured, then that's not a webserver issue.

So I think we should 

1) Use _re.type to determine which response class to use (avoids segfault due to no path)
2) Add the relevant path type logic to each class (i.e. set path = _re.paths.xxxxx in each class)
3) Let the CGI execute any script with the correct extension
4) Handle dependency installation inside the scripts
