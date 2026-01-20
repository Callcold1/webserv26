#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "../../Request/includes/utility_functions.hpp"
#include "../../Request/includes/core_def.hpp"

class Route
{

	struct targetInfo_s
	{
		bool inaccessible;
		bool is_dir;
		bool is_file;
		bool not_found;
		__mode_t permissions;
		std::string path;

		targetInfo_s() : inaccessible(false), is_dir(false), is_file(false), not_found(false), permissions(0), path("") {}
	};

private:
	std::vector<std::string>				allowed_methods;
	std::string								appended_root;
	bool									autoindex;
	bool									autoindex_set;
	std::string								index_file;
	std::string								path;
	int										redirect_status;
	std::string								redirect_url;
	std::string								root_dir;
	
	targetInfo_s							_target;
	std::map<std::string, targetInfo_s>		_routeCache;

public:
								Route();
								Route(const Route &other);
								Route operator=(const Route &another);
								~Route();

	// Setter functions
	void						setAllowedMethods(const std::vector<std::string> &allowed_methods);
	void						setAutoindex(const std::string& autoindex);
	void						setIndexFile(const std::string &index_file);
	void						setPath(const std::string &path);
	void						setRedirectStatus(int redirect_status);
	void						setRedirectUrl(const std::string &redirect_url);
	void						setRootDirRoute(const std::string &root_dir);
	
	// Getter functions
	bool						getAutoindexSet() const { return autoindex_set; }
	std::vector<std::string>	getAllowedMethods() const { return allowed_methods; }
	bool						getAutoindex() const { return autoindex; }
	std::string					getIndexFile() const { return index_file; }
	std::string					getPath() const { return path; }
	int							getRedirectStatus() const { return redirect_status; }
	std::string					getRedirectUrl() const { return redirect_url; }
	std::string					getRootDirRoute() const { return root_dir; }
	
	bool						allowsMethod(const std::string &method) const;
	int							checkRoute(Route *route);
	void						cleanRoute(Route *route);
	bool						containsTarget(const std::string &target) const;
	
	/* NEW FUNCTIONS */
	void						printRoute();
	std::ostream&				printRoute(std::ostream& os);
	std::ostream&				printTargetInfo(std::ostream& os);
	std::ostream&				printPermittedMethods(std::ostream &os);
	void						appendPathToRoot();	
	void						getTargetPathPermissions();
	void						setRouteTarget();
};
#endif
