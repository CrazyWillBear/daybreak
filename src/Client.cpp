#include "Client.hpp"

Client::Client(int socket, sockaddr_in clientAddress) : clientHandle(socket), clientAddress(clientAddress) {}

Client::~Client() = default;

std::string Client::ipAsString() const {
    return inet_ntoa(this->clientAddress.sin_addr);
}

auto Client::send(Response response) const -> std::expected<std::size_t, const char *> {
    std::string raw = response.build();

    std::size_t bytesWritten = ::send(this->clientHandle, raw.c_str(), raw.size(), 0);
    if (bytesWritten < 0)
        return std::unexpected("Failed to write bytes to client");

    return bytesWritten;
}

auto Client::recv() const -> std::expected<Request, const char *> {
    std::string requestData;

    while (true) {
        constexpr std::size_t bufferSize = 5120;
        char buffer[bufferSize] = {0};

        ssize_t receivedBytes = ::recv(this->clientHandle, buffer, bufferSize - 1, 0);
        if (receivedBytes < 0) {
            return std::unexpected("Failed to receive data from client: ");
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

auto Client::await(int serverHandle) -> std::expected<Client, const char *> {
    sockaddr_in clientAddress = {};
    socklen_t clientAddressSize = sizeof(clientAddress);

    int clientHandle = accept(serverHandle, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
    if (clientHandle < 0)
        return std::unexpected("Client failed to connect");

    Client client = Client(clientHandle, clientAddress);
    return client;
}

void Client::close() const {
    ::close(this->clientHandle);
}
