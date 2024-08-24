#pragma once

#include "ThreadPool.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>

class Server {
private:
    ThreadPool threadPool;

    sockaddr_in serverAddress;
    int serverHandle;

    unsigned short port;

public:
    explicit Server(unsigned short port);
    ~Server();

    void start();
};
