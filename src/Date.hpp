#pragma once

#include <ctime>
#include <expected>

typedef struct {} NoTime;

class Date {
private:
    std::time_t epoch;

    explicit Date(std::time_t epoch);

public:
    ~Date();

    /**
     * Construct a date from a unix epoch
     *
     * @param epoch C time_t value depicting a unix epoch time
     */
    static auto from_epoch(std::time_t epoch) -> std::expected<Date, NoTime>;
    static auto from_date(int year, int month, int day) -> std::expected<Date, NoTime>;
    static auto from_tm(std::tm timeInfo) -> std::expected<Date, NoTime>;
};
