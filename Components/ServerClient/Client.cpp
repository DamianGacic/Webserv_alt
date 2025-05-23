#include <unistd.h>
#include "../Headers/Client.hpp"

int Client::num_clients = 0; // static initialization, happens only once

// constructor + destructor
Client::Client(int fd)
	: _index(num_clients++), _fd(fd)
{}

Client::~Client(void)
{
	if (_fd >= 0)
        close(_fd);
}
// getters + setters
int Client::get_fd(void) const
{
	return _fd;
}

int Client::get_index(void) const
{
	return _index;
}

// pushes a string into the request_stream
void Client::pushRequestString(const std::string& request)
{
    _request_stream << request << std::endl;
}

// read one line from _request_stream into the buffer you provide
bool Client::getNextLine(std::string& request)
{
    return static_cast<bool>(std::getline(_request_stream, request));
}
// push response string to response stream
void Client::pushResponse(const std::string& response)
{
    _response_stream << response << std::endl;
}

// return the entire current content of the response stream as a string
std::string Client::getResponse() const
{
    return _response_stream.str();
}
// clear the response_stream and reset its state
void Client::clearResponseStream()
{
    _response_stream.str("");
    _response_stream.clear();
}