config rules to add

if (redirect.status && redirect.url.empty())
	throw std::runtime_error("Redirect status set to 'true' but no redirect URL provided");
