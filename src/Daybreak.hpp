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

#define TARGET(_name) Response _name(const Pattern &pattern, const Request &request)
#define ROUTE(_method, _path, _target) __attribute__((constructor)) void _router_##_target() { \
        Daybreak::addTarget(Pattern {.method = _method, .path = _path}, _target);\
    }


typedef struct {
    std::string method;
    std::string path;
} Pattern;

class Daybreak {
private:
    ThreadPool threadPool;

    sockaddr_in serverAddress;
    int serverHandle;

    unsigned short port;

    static std::vector<std::pair<Pattern, Response (*)(const Pattern &, const Request &)>> targets;

public:

    explicit Daybreak(unsigned short port);
    ~Daybreak();

    [[noreturn]]void start();

    static void addTarget(const Pattern &pattern, Response (*target)(const Pattern &, const Request &));
};
