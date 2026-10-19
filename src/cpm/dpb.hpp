#pragma once
#include <cstddef>
#include <cstdint>
namespace cpm22::cpm {
struct DiskParameterBlock {
    std::uint16_t sectors_per_track=26;
    std::uint8_t block_shift=3;
    std::uint8_t block_mask=7;
    std::uint8_t extent_mask=1;
    std::uint16_t max_block=242;
    std::uint16_t directory_max=63;
    std::uint16_t directory_allocation=0xC000;
    std::uint16_t check_count=63;
    std::uint16_t track_offset=2;
    [[nodiscard]] std::size_t block_size() const noexcept { return std::size_t{128} << block_shift; }
};
}