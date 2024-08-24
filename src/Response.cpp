#include "Response.hpp"

Response::Response(Status status, ResponseContent &content) :
    status(std::move(status)),
    responseContent(content) {
}

Response::~Response() {

}

std::string Response::build() {
    std::string statusLine = std::format("HTTP/1.1 {} {}", this->status.code, this->status.name);
    return std::format("{}\r\n"
"Server: river by gloggers\r\n"
"Content-Type: text/html\r\n"
"Content-Length: {}\n"
"Connection: Closed\r\n"
"\r\n"
"{}\r\n", statusLine, this->responseContent.length(), this->responseContent.raw());
}
