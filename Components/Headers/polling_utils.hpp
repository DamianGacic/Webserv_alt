#pragma once

#include "Client.hpp"
#include "Server.hpp"
#include <iostream>
#include <vector>

static void log_new_connection(int fd);
static void close_clients(std::map<int, Client>& clients);
static bool handle_connection_attempt(
    int fd,
    std::map<int, Server>& servers,
    std::map<int, Client>& clients,
    int epoll_fd
);
static void process_epoll_events(int epoll_fd,
                struct epoll_event* events, int active_fds,
                        std::map<int, Server>& servers,
                        std::map<int, Client>& clients);