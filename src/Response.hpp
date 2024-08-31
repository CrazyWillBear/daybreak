#pragma once

#include <cstddef>
#include <string>
#include <format>
#include <memory>
#include <vector>

#include "Cookie.hpp"

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
    [[nodiscard]] virtual std::string raw() const = 0;
    [[nodiscard]] virtual size_t length() const = 0;
    [[nodiscard]] virtual std::string mime() const noexcept = 0;
    virtual ~ResponseContent() = default;
};

class Response {
private:
    Status status;
    std::shared_ptr<ResponseContent> responseContent;

    std::vector<Cookie> cookies;

public:
    Response(const Status &status, std::shared_ptr<ResponseContent> content);
    Response(const Status &status, std::shared_ptr<ResponseContent> content, const std::vector<Cookie> &cookies);
    ~Response();

    [[nodiscard]] Status getStatus() const;
    [[nodiscard]] const ResponseContent &getResponseContent() const;

    void addCookie(const Cookie &cookie);

    std::string build();
};


