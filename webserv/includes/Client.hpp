#pragma once

#include <iostream>
#include <string>
#include <ctime>


enum State
{
	CONNECTED,
	READING_H,
	READING_B,
	PROCESSING,
	WRITING
};

// struct Request
// {
// 	std::string type = "";
//     std::string	raw_headers = "";
//     size_t		content_length = 0;
//     bool		chunked = false;
//     std::string content_type = "";
// 	std::string body = "";
// };

class Client
{
public:
	Client(int new_fd);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();

	void 		change_state(enum State state);
	void 		append_read(char *buffer, size_t len);
	void 		append_write(std::string str);
	std::string get_msg();
	void 		clear_out_buff();
	
	bool 		has_output;
	// logic for sending and receiving etc
private:
	int				fd;
	std::string		in_buff;
//	struct Request	request;
	std::string		out_buff;
	enum State		state;
	time_t			last_active;
};

