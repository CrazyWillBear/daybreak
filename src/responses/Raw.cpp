#include "Raw.hpp"

#include <utility>

Raw::Raw(std::string mimeType, std::string data) : mimeType(std::move(mimeType)), data(std::move(data)) {}
Raw::~Raw() = default;

std::string Raw::raw() const {
    return this->data;
}

size_t Raw::length() const {
    return this->data.size();
}

std::string Raw::mime() const noexcept {
    return this->mimeType;
}





