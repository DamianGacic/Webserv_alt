#include "../headers/polling.hpp"
#include "../headers/Client.hpp" //includes Server
#include "../headers/signals.hpp"
// #include "../headers/Webserv.hpp"  //eventually move all C++ ext libs here 
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>

static void log_new_connection(int fd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getpeername(fd, (struct sockaddr*)&addr, &len) == 0)
    {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));
        std::cout << "new connection at fd: " << fd
                  << " from " << ip << ":" << ntohs(addr.sin_port) << std::endl;
    }
    else
        std::cout << "new connection at fd: " << fd << "not found" << std::endl;
}
static bool handle_connection_attempt(
    int fd,
    std::map<int, Server>& servers,
    std::map<int, Client>& clients,
    int epoll_fd
)
{
    std::map<int, Server>::iterator server_it = servers.find(fd);
    if (server_it == servers.end())
        return false;
    int client_fd = accept(fd, NULL, NULL);
    if (client_fd < 0)
    {
        perror("accept");
        return false;
    }
    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    Client new_client(client_fd, server_it->second);
    clients.insert(std::make_pair(client_fd, new_client));    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = client_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
    {
        perror("epoll_ctl: add client");
        close(client_fd);
        clients.erase(client_fd);
        return false;
    }
    if (DEBUG)
        log_new_connection(client_fd);    return true;
}
static void close_clients(std::map<int, Client>& clients)
{
    std::map<int, Client>::iterator it;
    for (it = clients.begin(); it != clients.end(); ++it) {
        std::string response = "Connection closed.";
        send(it->first, response.c_str(), response.size(), MSG_NOSIGNAL);
        close(it->first);
    }
}
static void process_epoll_events(int epoll_fd, struct epoll_event* events, int active_fds,
                                 std::map<int, Server>& servers,
                                 std::map<int, Client>& clients)
{
    for (int i = 0; i < active_fds; ++i)
    {
        int fd = events[i].data.fd;
        uint32_t revents = events[i].events;
        if (revents & EPOLLIN || revents & EPOLLHUP)
        {
            if (servers.find(fd) != servers.end())
                handle_connection_attempt(fd, servers, clients, epoll_fd);
        }
    }
}