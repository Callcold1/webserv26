#include "Client.hpp"

Client::Client(int new_fd)
{
	fd = new_fd;
	//in_buff = "";
	out_buff = "";
	state = CONNECTED;
	time(&last_active);

	has_output = false;
}

Client::Client(const Client &other)
    : 
	  has_output(other.has_output),
	  fd(other.fd),
     // in_buff(other.in_buff),
      out_buff(other.out_buff),
      state(other.state),
      last_active(other.last_active)
{}
Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		fd = other.fd;
		//in_buff = other.in_buff;
		out_buff = other.out_buff;
		state = other.state;
		last_active = other.last_active;
		has_output = other.has_output;
	}
	return *this;
}
Client::~Client()
{
}

void change_state(enum State state);
void Client::append_read(char *buffer, size_t len)
{
	(void)buffer;
	(void)len;
	if (state == CONNECTED)
		state = READING_H;
	if (state == READING_H)
	{
		//in_buff.append(buffer, len);
		//size_t pos = in_buff.find("/r/n/r/n");
		//if (pos != std::string::npos)
		//{
		//	//request.raw_headers.append(in_buff, 0, pos);
		//	// fill request struct
		//	in_buff.erase(0, pos + 4);
		//	state = READING_B; //if body = 0 jump to next
		//	append_write("full header received");
		//}
		append_write("msg received");
	}
	else if (state == READING_B)
	{
		//in_buff.append(buffer, len);
		//more body logic here
	}
}

void Client::append_write(std::string str)
{
	out_buff.append(str);
	has_output = true;
}

std::string Client::get_msg()
{
	return(out_buff);
}

void Client::clear_out_buff()
{
	out_buff = "";
	has_output = false;
}