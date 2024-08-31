#include "Cookie.hpp"

#include <utility>
#include <sstream>

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

