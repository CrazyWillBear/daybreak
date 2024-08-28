#include "Server.hpp"
#include "HTML.hpp"
#include "Response.hpp"
#include "Client.hpp"
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
        throw std::runtime_error("Failed to initialize clientHandle file descriptor");

    int bindStatus = bind(
            serverHandle, 
            reinterpret_cast<struct sockaddr*>(&this->serverAddress), 
            sizeof(this->serverAddress)
    );

    if (bindStatus < 0) {
        throw std::runtime_error("Failed to bind serverHandle");
    }
}

Server::~Server() {
    std::cerr << "Shutting down server handle.\n";
    close(this->serverHandle);
}

void Server::start() {
    listen(this->serverHandle, 5);
    std::cout << "Server started and listening on port: " << this->port << "\n";
    std::cout << "Waiting for connections...\n";
    
    while (true) {
        auto client = Client::await(this->serverHandle).value();

        std::cout << "Client connected from: " << client.ipAsString() << "\n";

        this->threadPool.enqueue([client, this]() {
            auto data = client.recv().value();
            const Method& requestMethod = data.getMethod();

            for (const auto &target : Server::targets) {
                if (target.first.method == requestMethod.method && target.first.path == requestMethod.path) {
                    const auto bytesWritten = client.send(target.second(target.first));
                    (void) bytesWritten;

                    client.close();

                    return;
                }
            }

            std::shared_ptr<ResponseContent> content = std::make_shared<HTML>(
                $ html({
                    $ head({
                        $ title("daybreak")
                    }),
                    $ body(attrib {._style = "color: red;"}, {
                        $ h1("daybreak 404"),
                    })
                })
            );


            Response response(Responses::OK, content);
            const auto bytesWritten = client.send(response);
            (void) bytesWritten;

            client.close();
        });
    }
}

void Server::addTarget(const Pattern &pattern, Response (*target)(const Pattern &)) {
    targets.emplace_back(pattern, target);
}
