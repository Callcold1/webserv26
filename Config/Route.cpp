#include "./includes/Route.hpp"

// Constructor
Route::Route() 	: 
allowed_methods(),
appended_root(""),
autoindex(false),
autoindex_set(false),
index_file(""),
path(""),
redirect_status(),
redirect_url(""),
root_dir(""){}

// Copy Constructor
Route::Route(const Route &other)
	: allowed_methods(other.allowed_methods),
	  appended_root(other.appended_root),
	  autoindex(other.autoindex),
	  autoindex_set(other.autoindex_set),
	  index_file(other.index_file),
	  path(other.path),
	  redirect_status(other.redirect_status),
	  redirect_url(other.redirect_url),
	  root_dir(other.root_dir) {}

// Assignment Operator
Route Route::operator=(const Route &another)
{
	if (this != &another)
	{
		allowed_methods = another.allowed_methods;
		autoindex = another.autoindex;
		autoindex_set = another.autoindex_set;
		index_file = another.index_file;
		path = another.path;
		redirect_status = another.redirect_status;
		redirect_url = another.redirect_url;
		root_dir = another.root_dir;
	}
	return *this;
}

// Destructor
Route::~Route() {}

// Setters
void Route::setPath(const str_t &path) { this->path = path; }
void Route::setAllowedMethods(const std::vector<str_t> &allowed_methods) { this->allowed_methods = allowed_methods; }
void Route::setRootDirRoute(const str_t &root_dir) { this->root_dir = root_dir; }
void Route::setIndexFile(const str_t &index_file) { this->index_file = index_file; }

void Route::setAutoindex(const str_t &autoindex)
{
	autoindex_set = true;
	if (autoindex.find("on") == 0)
	{
		this->autoindex = true;
	}
	else if (autoindex.find("off") == 0)
	{
		this->autoindex = false;
	}
	else
		autoindex_set = false;
}
void Route::setRedirectStatus(int redirect_status) { this->redirect_status = redirect_status; }
void Route::setRedirectUrl(const str_t &redirect_url) { this->redirect_url = redirect_url; }

// Check if route contains the target path
bool Route::containsTarget(const str_t &requestStr) const
{
	if (path.compare(requestStr) == 0)
		return true;
	else if (_target.is_file)
		return false; // ONLY RETURN TRUE FOR A FILE WHEN AN EXACT MATCH IS FOUND
	else if (_target.is_dir){
		//NEED TO DECIDE IF WE ALLOW PARTIAL MATCHES OR NOT
		// 1. I've removed last segment of the path
		return true;
	}
	return false;
}

// Check if method is allowed
bool Route::allowsMethod(const str_t &method) const
{
	for (size_t i = 0; i < allowed_methods.size(); i++)
	{
		if (allowed_methods[i] == method)
			return true;
	}
	return false;
}

// Route validation check
int Route::checkRoute(Route *route)
{
	if (route->path.empty())
		return 0;
	return 1;
}

// Cleanup
void Route::cleanRoute(Route *route)
{
	route->allowed_methods.clear();
}

// Print allowed methods
std::ostream &Route::printPermittedMethods(std::ostream &os)
{
	for (size_t i = 0; i < allowed_methods.size(); i++)
	{
		os << allowed_methods[i];
		if (i < allowed_methods.size() - 1)
			os << ", ";
		else
			os << std::endl;
	}
	return os;
}

// Print TargetInfo
std::ostream& Route::printTargetInfo(std::ostream &os)
{
	os << "Target Inaccessible: " << _target.inaccessible << std::endl;
	os << "Target Is Directory: " << _target.is_dir << std::endl;
	os << "Target Is File: " << _target.is_file << std::endl;
	os << "Target Not Found: " << _target.not_found << std::endl;
	os << "Target Appended Path: " << _target.path << std::endl;
	os << "Target Permissions: "
	<< std::oct << std::setfill('0') << std::setw(4) << (_target.permissions & 0777) 
	<< std::dec << std::setfill(' ') << std::setw(0) << std::endl;
	return os;
}

// Print route details
std::ostream &Route::printRoute(std::ostream &os)
{
	os << "Path: " << path << "\n"
	   << "Root Dir: " << root_dir << "\n"
	   << "Index File: " << index_file << "\n"
	   << "Autoindex: " << (autoindex ? "On" : "Off") << "\n"
	   << "Redirect Status: " << redirect_status << "\n"
	   << "Redirect URL: " << redirect_url << "\n";
	printTargetInfo(os);
	printPermittedMethods(os);
	return os;
}

void Route::printRoute()
{
	std::cout << "Path: " << path << "\n"
			  << "Root Dir: " << root_dir << "\n"
			  << "Index File: " << index_file << "\n"
			  << "Autoindex: " << (autoindex ? "On" : "Off") << "\n"
			  << "Redirect Status: " << redirect_status << "\n"
			  << "Redirect URL: " << redirect_url << "\n";
}

/* 
	Function to append endpoint to route->root 
	Saved to _target.path

	The _target_path is to be used when accessing endpoints during requests
*/

void Route::appendPathToRoot()
{
	str_t r = root_dir;
	str_t p = path;

	if (r[r.size() - 1] == '/')
		trimEndChar(r, '/');
	if (p[0] == '/')
		trimLeadingChar(p, '/');
	_target.path = r + '/' + p;
}

/* Route Validation */

/* 
	This is partial validation, to be performed on each Route.

	The overall purpose is to have upfront check to :
		: build the path for the endpoint (path)
		: check that the path is accessible
		: save path permissions
		: check if path is a file, or a directory

	If we modify or create files, maybe we have to refresh this for each Route
	
	Otherwise, it is done at outset, and doesn't need to be checked again for every
	request.

	If a Route is used with a DELETE or POST request, then we should refresh the Route
	Should we maintain a map of Routes for all servers to make this easier to manage?

*/

/*
	This struct has been added to the Route class
	An instance should be used as a private variable in the Route class
	
	struct targetInfo_s
	{
		bool inaccessible;		// set to true if path can"t be accessed
		bool is_dir;			// set to true if path is a dir
		bool is_file;			// set to true if path is a file
		bool not_found;			// set to true if file not found
		__mode_t permissions;	// the file permissions
		std::string path;		// the path (root + '/' endpoint )

		targetInfo_s() : inaccessible(false), is_dir(false), is_file(false), not_found(false), permissions(0), path("") {}
	};
*/
void Route::getTargetPathPermissions(){
	struct stat fileStat;
	
	if (stat(_target.path.c_str(), &fileStat) == 0)
	{
		_target.is_dir = S_ISDIR(fileStat.st_mode);
		_target.is_file = S_ISREG(fileStat.st_mode);
		_target.permissions = fileStat.st_mode;
	}
	else if(errno == ENOENT)
		_target.not_found = true;
	else if(errno == EACCES)
		_target.inaccessible = true;
}

void Route::setRouteTarget()
{
	appendPathToRoot();
	getTargetPathPermissions();
	/* 
		Need to decide if we throw error if path inaccessible or not found
		 or just log it?
	*/
	return;
}