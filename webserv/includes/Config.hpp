#pragma once

class Config {

	public:

		/* Constructors and destructor */
		Config(Config &config);
		Config(const Config& other);
		Config&	operator=( const Config& other);
		~Config(void);

		int ports[];

};