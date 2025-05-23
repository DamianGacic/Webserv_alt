#pragma once

# include <iostream>
# include <sstream>
# include <string>
# include "Server.hpp"

class Client
{
	private:
		static int                      num_clients; // keeps track of all created clients
		int	const                       _index;     // nth client. set at creation from num_clients
		int const                       _fd;		// fd. passed at construction
		std::stringstream               _request_stream;
		std::stringstream               _response_stream;
		
	public:
		Client(int fd);
		~Client(void);

		int	get_fd( void ) const;
		int	get_index( void ) const;
		// pushes a string into the request_stream
		void pushRequestString(const std::string& request);
		// read one line from _request_stream into the buffer you provide
		bool getNextLine(std::string& request);
		// add response string to response stream
		void pushResponse(const std::string& response);
		// return the entire current content of the response stream as a string
		std::string getResponse() const;
		// clear the response_stream and reset its state
		void clearResponseStream();
};