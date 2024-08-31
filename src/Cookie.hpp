#pragma once

#include <map>
#include <optional>
#include <string>

#include "Date.hpp"

enum class SameSite {
    Strict,
    Lax,
    None
};


typedef struct {
    /* Defines the host to which the cookie will be sent */
    std::optional<std::string> domain;
    /* Indicates the date the cookie will expire */
    std::optional<Date> date;
    /* Forbids JavaScript from accessing the cookie, ex: through Document.cookie() */
    std::optional<bool> httpOnly;
    /* Indicates the number of seconds until the cookie expires, zero or
     * negative will expire the cookie immediately */
    std::optional<int> maxAge;
    /* Indicates that the cookie should be stored using partitioned storage */
    std::optional<bool> partitioned;
    /* Indicates the path that must exist in the requested URL for the browser to send the Cookie header */
    std::optional<std::string> path;
    /* Controls whether a cookie is sent with cross-site requests */
    std::optional<SameSite> sameSite;
    /* Indicates whether the cookie is secure */
    std::optional<bool> secure;
} CookieAttributes;

class Cookie {
private:
    CookieAttributes attributes;

    std::string name, value;

public:
    Cookie(std::string name, std::string value, CookieAttributes attributes = {});
    explicit Cookie(const std::pair<std::string, std::string>& detail, const CookieAttributes &attributes = {});
    ~Cookie();

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getValue() const;

    [[nodiscard]] std::string build() const;
};
