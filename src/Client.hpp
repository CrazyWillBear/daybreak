#pragma once

#include <expected>
#include <string>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "Response.hpp"
#include "Request.hpp"

class Client {
private:
    sockaddr_in clientAddress;

    int clientHandle;

    Client(int socket, sockaddr_in clientAddress);

public:
    ~Client();

    [[nodiscard]] std::string ipAsString() const;

    void close() const;

    auto send(Response response) const -> std::expected<std::size_t, const char *>;

    /**
     * Receive amount of bytes from client
     * 
     * @tparam S amount of bytes to buffer and return
     */
    auto recv() const -> std::expected<Request, const char *> ;

    static auto await(int serverHandle) -> std::expected<Client, const char *>;
};
