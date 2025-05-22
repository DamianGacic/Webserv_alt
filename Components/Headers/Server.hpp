#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Config.hpp"
#include "Location.hpp"

class Server : public Config
{
	private:
		static int                      servers_num; //single attributes
		bool                            _active;
		std::string                     _server_name;
		std::string                     _ip;
		
		std::vector<int>                _ports;		//maps & vector attributes
		std::map<std::string, Location>	_locations;
		std::vector<int>                _sockets;

	public:
	//constr destr
		Server( void );
		~Server( void );

		//getters setters
		//single attributes
		int									is_active( void ); //int instead bool because -1 for error check
		int									get_server_num( void );
		std::string							get_server_name( void ) const;
		void								set_server_name( std::string server_name ); //preliminary, for setup only
		std::string							get_ip( void ) const;
		void								set_ip( std::string ip );
		//maps & vector attributes
		std::vector<int>					get_ports( void ) const;
		void								add_port( int port );
		void								no_port( int port );	//only used in setup for now
		std::map<std::string, Location>		get_locations( void ) const;
		std::pair<bool, Location const*>	get_location( std::string route ) const;
		void								add_location( std::string route, Location location );
		std::vector<int>					get_sockets( void ) const;
		void								add_socket( int socket_fd ); //preliminary
		//methods
		void								run( void ); //start listening on all ports & servers
		void								shutdown( void ); //shuts down safely
};