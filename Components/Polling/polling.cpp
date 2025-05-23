#include "../headers/polling.hpp"
#include "../headers/Client.hpp" //includes Server
#include "../headers/signals.hpp"
// #include "../headers/Webserv.hpp"  //eventually move all C++ ext libs here 
#include "../headers/polling_utils.hpp"
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

#define MAX_EVENTS 128

void polling(std::vector<Server> servers_vector)
{
    std::map<int, Server> servers;
    std::map<int, Client> clients;
    std::vector<Server>::const_iterator serv_it;
    for (serv_it = servers_vector.begin(); serv_it != servers_vector.end(); ++serv_it)
    {
        std::vector<int> sockets = serv_it->get_sockets();
        std::vector<int>::const_iterator sock_it;
        for (sock_it = sockets.begin(); sock_it != sockets.end(); ++sock_it)
        {
            servers[*sock_it] = *serv_it;
        }
    }
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        perror("epoll_create1");
        return;
    }
    std::map<int, Server>::iterator it;
    for (it = servers.begin(); it != servers.end(); ++it)
    {
        struct epoll_event event;
        event.events = EPOLLIN;
        event.data.fd = it->first;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, it->first, &event) == -1)
        {
            perror("epoll_ctl: add server fd");
        }
    }    struct epoll_event events[MAX_EVENTS];
    int count;    while (!sigint_signal)
    {
        if (DEBUG)
        {
            std::cout << "\n--epoll: listening... ("
                      << servers.size() << " servers, "
                      << clients.size() << " clients)\n";
        }
        count = epoll_wait(epoll_fd, events, MAX_EVENTS, POLL_TIMEOUT);
        if (count < 0)
        {
            if (!sigint_signal)
                perror("epoll_wait");
            break;
        }
        if (count == 0)
            continue;
        process_epoll_events(epoll_fd, events, count, servers, clients);
    }
    close_clients(clients);
    close(epoll_fd);
}