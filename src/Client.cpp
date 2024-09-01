/*
This code is written by ChatGPT 4.0
*/


#include "Client.hpp"
#include <cstring> // For strerror

Client::Client(int socket, sockaddr_in clientAddress) 
    : clientHandle(socket), clientAddress(clientAddress) {}

Client::~Client() {
    if (clientHandle >= 0) {
        ::close(clientHandle);
    }
}

std::string Client::ipAsString() const {
    return inet_ntoa(this->clientAddress.sin_addr);
}

auto Client::send(const Response& response) const -> std::expected<std::size_t, std::string> {
    std::string raw = response.build();

    ssize_t bytesWritten = ::send(this->clientHandle, raw.c_str(), raw.size(), 0);
    if (bytesWritten < 0)
        return std::unexpected(std::string("Failed to write bytes to client: ") + strerror(errno));

    return bytesWritten;
}

auto Client::recv() const -> std::expected<Request, std::string> {
    std::string requestData;

    while (true) {
        constexpr std::size_t bufferSize = 5120;
        char buffer[bufferSize] = {0};

        ssize_t receivedBytes = ::recv(this->clientHandle, buffer, bufferSize, 0);
        if (receivedBytes < 0) {
            return std::unexpected(std::string("Failed to receive data from client: ") + strerror(errno));
        }

        if (receivedBytes == 0) {
            return std::unexpected("Connection closed by client");
        }

        requestData.append(buffer, receivedBytes);

        if (requestData.find('\n') != std::string::npos) {
            break;
        }
    }

    return Request(requestData);
}

auto Client::await(int serverHandle) -> std::expected<Client, std::string> {
    sockaddr_in clientAddress = {};
    socklen_t clientAddressSize = sizeof(clientAddress);

    int clientHandle = accept(serverHandle, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
    if (clientHandle < 0)
        return std::unexpected(std::string("Client failed to connect: ") + strerror(errno));

    return Client(clientHandle, clientAddress);
}

void Client::close() const {
    if (clientHandle >= 0) {
        ::close(this->clientHandle);
    }
}
