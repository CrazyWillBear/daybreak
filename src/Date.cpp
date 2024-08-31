#include "Date.hpp"

Date::Date(const std::time_t epoch) :
    epoch(epoch) {}

auto Date::from_epoch(const std::time_t epoch) -> std::expected<Date, NoTime> {
    if (epoch == static_cast<std::time_t>(-1))
        return std::unexpected(NoTime {});

    return Date(epoch);
}

auto Date::from_date(const int year, const int month, const int day) -> std::expected<Date, NoTime> {
    std::tm timeInfo = {};
    timeInfo.tm_year = year - 1900;
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;

    const time_t epoch = std::mktime(&timeInfo);

    return Date::from_epoch(epoch);
}

auto Date::from_tm(std::tm timeInfo) -> std::expected<Date, NoTime> {
    const time_t epoch = std::mktime(&timeInfo);

    return Date::from_epoch(epoch);
}

Date::~Date() = default;

