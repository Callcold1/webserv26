#include "Client.hpp"

Client::Client(int new_fd)
{
	fd = new_fd;
	in_buff = "";
	out_buff = "";
	state = CONNECTED;
	time(&last_active);
}

Client::Client(const Client &other)
    : fd(other.fd),
      in_buff(other.in_buff),
      out_buff(other.out_buff),
      state(other.state),
      last_active(other.last_active)
{}
Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		fd = other.fd;
		in_buff = other.in_buff;
		out_buff = other.out_buff;
		state = other.state;
		last_active = other.last_active;
	}
	return *this;
}
Client::~Client()
{
}