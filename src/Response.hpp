#pragma once

#include <cstddef>
#include <string>
#include <format>

typedef struct {
    unsigned int code;
    const char *name;
} Status;

/**
 *  Static status definitions for use in Response
 */
class Responses {
public:
    static constexpr Status OK = {
        .code = 200,
        .name = "OK"
    };

    static constexpr Status NOT_FOUND = {
        .code = 404,
        .name = "Not Found"
    };
};

class ResponseContent {
public:
    virtual std::string raw() = 0;
    virtual size_t length() = 0;
    virtual ~ResponseContent() = default;
};

class Response {
private:
    Status status;
    ResponseContent &responseContent;

public:
    Response(Status status, ResponseContent &content);
    ~Response();

    std::string build();
};

