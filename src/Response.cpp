#include "Response.hpp"

#include <iostream>
#include <utility>

Response::Response(const Status &status, std::shared_ptr<ResponseContent> content) :
    status(status),
    responseContent(std::move(content)) {
}

Response::Response(const Status &status, std::shared_ptr<ResponseContent> content, const std::vector<Cookie> &cookies) :
    status(status),
    responseContent(std::move(content)),
    cookies(cookies) {
}

Response::~Response() = default;

Status Response::getStatus() const {
    return this->status;
}

const ResponseContent &Response::getResponseContent() const {
    // FIXME: this is probably extremely unsafe
    return *this->responseContent;
}

void Response::addCookie(const Cookie &cookie) {
    this->cookies.emplace_back(cookie);
}


std::string Response::build() {
    std::string statusLine = std::format("HTTP/1.1 {} {}", this->status.code, this->status.name);
    std::string cookieList;

    for (const auto &cookie : this->cookies) {
        cookieList.append(cookie.build());
    }

    return std::format("{}\r\n"
"Server: daybreak by gloggers\r\n"
"Content-Type: {}\r\n"
"Content-Length: {}\n"
"Connection: Closed\r\n"
"{}"
"\r\n"
"{}\r\n", statusLine, this->responseContent->mime(), this->responseContent->length(), cookieList, this->responseContent->raw());
}
