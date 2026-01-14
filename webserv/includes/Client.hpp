#pragma once

#include <iostream>
#include <string>
#include <ctime>


enum State
{
	CONNECTED,
	READING,
	PROCESSING,
	WRITING
};

class Client
{
public:
	Client(int new_fd);
	Client(const Client &other);
	Client &operator=(const Client &other);
	~Client();

	void change_state(enum State state);
	// logic for sending and receiving etc
private:
	int fd;
	std::string in_buff;
	std::string out_buff;
	enum State state;
	time_t last_active;
};

