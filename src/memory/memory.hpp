#pragma once
#include <array>
#include <cstdint>
namespace cpm22::memory {
class Memory {
public:
    static constexpr std::size_t size = 65536;
    Memory() = default;
    std::uint8_t read(std::uint16_t a) const noexcept { return data_[a]; }
    void write(std::uint16_t a,std::uint8_t v) noexcept { data_[a]=v; }
    void clear() noexcept { data_.fill(0); }
    std::uint8_t* data() noexcept { return data_.data(); }
    const std::uint8_t* data() const noexcept { return data_.data(); }
private: std::array<std::uint8_t,size> data_{};
};
}
