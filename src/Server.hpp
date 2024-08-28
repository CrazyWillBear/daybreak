#pragma once

#include "ThreadPool.hpp"
#include "Response.hpp"
#include "Request.hpp"

#include <iostream>
#include <cstring>
#include <stdexcept>
#include <map>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct {
    std::string method;
    std::string path;
} Pattern;

class Server {
private:
    ThreadPool threadPool;

    sockaddr_in serverAddress;
    int serverHandle;

    unsigned short port;

    std::vector<std::pair<Pattern, Response (*)(const Pattern &)>> targets;

public:

    explicit Server(unsigned short port);
    ~Server();

    [[noreturn]]void start();

    void addTarget(const Pattern &pattern, Response (*target)(const Pattern &));
};
