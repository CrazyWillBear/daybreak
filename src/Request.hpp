#pragma once

#include <cstring>
#include <expected>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

inline std::string trim(const std::string &str) {
    auto start = str.begin();
    while (start != str.end() && std::isspace(*start)) {
        start++;
    }

    auto end = str.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return {start, end + 1};
}

struct Method {
    std::string method;
    std::string path;
    std::string version;
};

class Methods {
public:
    static constexpr std::string GET = "GET";
    static constexpr std::string POST = "POST";
};

inline std::istream &operator>>(std::istream &in, Method &rhs) {
    in >> rhs.method >> rhs.path >> rhs.version;
    return in;
}

inline std::ostream &operator<<(std::ostream &out, const Method &rhs) {
    out << rhs.method << " " << rhs.path << " " << rhs.version;
    return out;
}

struct Header {
    std::string name;
    std::string value;
};

inline std::istream &operator>>(std::istream &in, Header &rhs) {
    std::string line;
    std::getline(in, line);

    std::stringstream ss(line);
    std::getline(ss, rhs.name, ':');
    std::getline(ss, rhs.value);

    rhs.name = trim(rhs.name);
    rhs.value = trim(rhs.value);

    return in;
}

inline std::ostream &operator<<(std::ostream &out, const Header &rhs) {
    out << rhs.name << " " << rhs.value;

    return out;
}

class Request {
private:
    std::string rawRequest;

    Method method;
    std::vector<Header> headers;

public:
    explicit Request(std::string rawRequest);
    ~Request();

    [[nodiscard]] const Method &getMethod() const;
    auto getHeader(const std::string& name) -> std::expected<Header, const char *>;

};

