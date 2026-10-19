#pragma once
#include <array>
#include <cstdint>
#include <string>
namespace cpm22::cpm {
struct Fcb {
    std::uint8_t user = 0;
    std::array<char,8> name{};
    std::array<char,3> extension{};
    std::uint8_t extent = 0;
    std::uint8_t reserved = 0;
    std::uint8_t record_count = 0;
    std::array<std::uint8_t,16> allocation{};
    static Fcb from_string(const std::string& value, std::uint8_t user=0);
    [[nodiscard]] std::string filename() const;
};
}