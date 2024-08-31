#include "Cookie.hpp"

#include <utility>
#include <sstream>
#include <map>

/* for trim() */
#include "Request.hpp"

std::map<SameSite, std::string> SameSiteMap = {
    { SameSite::Strict, "Strict" },
    { SameSite::Lax, "Lax" },
    { SameSite::None, "None" }
};

Cookie::Cookie(std::string name, std::string value, CookieAttributes attributes) :
    attributes(std::move(attributes)),
    name(std::move(name)),
    value(std::move(value)) {
}

Cookie::Cookie(const std::pair<std::string, std::string> &detail, const CookieAttributes &attributes) :
    Cookie(detail.first, detail.second, attributes) {}

Cookie::~Cookie() = default;

std::string Cookie::getName() const {
    return this->name;
}

std::string Cookie::getValue() const {
    return this->value;
}

std::string Cookie::build() const {
    std::stringstream attributeStream("");

    if (this->attributes.domain)
        attributeStream << "; Domain=" << this->attributes.domain.value();

    if (this->attributes.date) {}

    if (this->attributes.httpOnly)
        attributeStream << "; HttpOnly";

    if (this->attributes.maxAge)
        attributeStream << "; MaxAge=" << this->attributes.maxAge.value();

    if (this->attributes.partitioned)
        attributeStream << "; Partitioned";

    if (this->attributes.path)
        attributeStream << "; Path=" << this->attributes.path.value();

    if (this->attributes.sameSite)
        attributeStream << "; Site=" << SameSiteMap[SameSite::Lax];

    if (this->attributes.secure)
        attributeStream << "; Secure";

    if (attributeStream.str().empty())
        return std::format("Set-Cookie: {}={}\r\n", this->name, this->value);

    return std::format("Set-Cookie: {}={}{}\r\n", this->name, this->value, attributeStream.str());
}

auto Cookie::from_string(const std::string &value) -> std::vector<Cookie> {
    std::vector<Cookie> cookies {};

    // Remove "Cookie: " part
    std::string header;
    std::stringstream cookieStream(value);
    std::getline(cookieStream, header, ':');

    std::string section;
    // Skip leading spaces after the colon
    std::getline(cookieStream >> std::ws, section);

    // Split into key-value pairs by ';'
    std::stringstream sectionStream(section);
    std::string cookiePart;
    while (std::getline(sectionStream, cookiePart, ';')) {
        // Trim leading and trailing spaces
        cookiePart.erase(0, cookiePart.find_first_not_of(" \t"));
        cookiePart.erase(cookiePart.find_last_not_of(" \t") + 1);

        // Split by '='
        if (size_t pos = cookiePart.find('='); pos != std::string::npos) {
            std::string key = cookiePart.substr(0, pos);
            std::string val = cookiePart.substr(pos + 1);

            // Trim spaces from key and value
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            val.erase(0, val.find_first_not_of(" \t"));
            val.erase(val.find_last_not_of(" \t") + 1);

            cookies.emplace_back(key, val);
        }
    }

    return cookies;
}
