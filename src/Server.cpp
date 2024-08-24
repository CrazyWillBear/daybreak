#include "Server.hpp"
#include "HTML.hpp"
#include "Response.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <memory>
#include <sys/socket.h>

Server::Server(unsigned short port) : threadPool(5), port(port) {
    memset(&this->serverAddress, 0, sizeof(this->serverAddress));

    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    this->serverAddress.sin_port = htons(this->port);

    this->serverHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (serverHandle < 0)
        throw std::runtime_error("Failed to initialize socket file descriptor");

    int bindStatus = bind(
            serverHandle, 
            reinterpret_cast<struct sockaddr*>(&this->serverAddress), 
            sizeof(this->serverAddress)
    );

    if (bindStatus < 0)
        throw std::runtime_error("Failed to bind socket");
}

Server::~Server() {
    std::cerr << "Shutting down server handle.\n";
    close(this->serverHandle);
}

void Server::start() {
    listen(this->serverHandle, 1);
    std::cout << "Server started and listening on port: " << this->port << "\n";
    std::cout << "Waiting for connections...\n";
    
    while (true) {
        sockaddr_in clientAddress;
        socklen_t clientAddressSize = sizeof(clientAddress);

        int clientHandle = accept(this->serverHandle, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
        if (clientHandle < 0) {
            std::cerr << "Client failed to connect, skipping.\n";
            continue;
        }

        char *clientIP = inet_ntoa(clientAddress.sin_addr);
        std::cout << "Client connected from: " << clientIP << "\n";

        this->threadPool.enqueue([&clientHandle]() {
            constexpr size_t bufferSize = 5120;
            char buffer[bufferSize];

            int recievedBytes = recv(clientHandle, &buffer, bufferSize, 0);
            if (recievedBytes == 0) {
                std::cerr << "Failed to recieve bytes from client\n";
                return;
            }

            buffer[bufferSize - 1] = '\0';

            HTML content(
                $ html({
                    $ head({
                        $ title("river")
                    }),
                    $ body(attrib {._style = "color: red;"}, {
                        $ h1("river test"),
                        $ p("line 2")
                    })
                })
            );

            Response response(Responses::OK, content);
            std::string raw = response.build();

            size_t bytesWritten = send(clientHandle, raw.c_str(), raw.size(), 0);
            if (bytesWritten < 0) {
                std::cerr << "Failed to send bytes to client\n";
                return;
            }

            close(clientHandle);
        });
    }
}
