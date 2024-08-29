#include "Response.hpp"

#include <utility>

Response::Response(const Status &status, std::shared_ptr<ResponseContent> content) :
    status(status),
    responseContent(std::move(content)) {
}

Response::~Response() = default;

Status Response::getStatus() const {
    return this->status;
}

const ResponseContent &Response::getResponseContent() const {
    return *this->responseContent;
}

std::string Response::build() {
    std::string statusLine = std::format("HTTP/1.1 {} {}", this->status.code, this->status.name);
    return std::format("{}\r\n"
"Server: daybreak by gloggers\r\n"
"Content-Type: {}\r\n"
"Content-Length: {}\n"
"Connection: Closed\r\n"
"\r\n"
"{}\r\n", statusLine, this->responseContent->mime(), this->responseContent->length(), this->responseContent->raw());
}
