#include "Daybreak.hpp"
#include "responses/HTML.hpp"
#include "Response.hpp"
#include "Client.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <memory>
#include <sys/socket.h>


std::vector<std::pair<Pattern, Response (*)(const Pattern &, const Request &)>> Daybreak::targets;
std::unique_ptr<Daybreak> Daybreak::instance = std::unique_ptr<Daybreak>(new Daybreak(7878));

Daybreak::Daybreak(const unsigned short port) : threadPool(5), serverAddress({}), port(port) {

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

Daybreak::~Daybreak() {
    std::cout << "Daybreak shutting down!!\n";
    close(this->serverHandle);
}

void Daybreak::start() {
    listen(this->serverHandle, 5);
    std::cout << "Server started and listening on port: " << this->port << "\n";
    std::cout << "Waiting for connections...\n";

    while (true) {
        auto client = Client::await(this->serverHandle).value();

        std::cout << "Client connected from: " << client.ipAsString() << "\n";

        this->threadPool.enqueue([client, this]() {
            const Request data = client.recv().value();
            const auto&[method, path, version] = data.getMethod();

            for (const auto &[fst, snd] : Daybreak::targets) {
                if (fst.method == method && fst.path == path) {
                    const auto bytesWritten = client.send(snd(fst, data));
                    (void) bytesWritten;

                    client.close();

                    return;
                }
            }

            const std::shared_ptr<ResponseContent> content = std::make_shared<HTML>(
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

void Daybreak::addTarget(const Pattern &pattern, Response (*target)(const Pattern &, const Request &)) {
    targets.emplace_back(pattern, target);
}
