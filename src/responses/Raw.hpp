#pragma once
#include "Response.hpp"

class Raw final : public ResponseContent {
private:
    std::string mimeType;
    std::string data;

public:
    Raw(std::string mimeType, std::string data);
    ~Raw() override;

    [[nodiscard]] std::string raw() const override;
    [[nodiscard]] size_t length() const override;
    [[nodiscard]] std::string mime() const noexcept override;
};
