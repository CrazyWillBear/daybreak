#include "Request.hpp"

Request::Request(std::string rawRequest) : rawRequest(std::move(rawRequest)), method({}) {
    std::stringstream ss(this->rawRequest);

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(ss, line))
        lines.emplace_back(line);

    // extract method:
    std::stringstream headerStream(lines[0]);

    headerStream >> this->method;

    // extract headers:
    for (size_t i = 1; i < lines.size(); ++i) {
        std::stringstream lineStream(lines[i]);

        Header header = {};
        lineStream >> header;

        std::cout << header << '\n';

        this->headers.emplace_back(header);
    }
}

Request::~Request() = default;

const Method & Request::getMethod() const {
    return this->method;
}

auto Request::getHeader(const std::string& name) -> std::expected<Header, const char *> {
    for (auto &header : this->headers) {
        if (header.name == name) {
            return header;
        }
    }

    return std::unexpected("Header not found!");
}
