#pragma once

#include <array>
#include <cstdint>

namespace cpm22::platform {

class CmosRtc {
public:
    CmosRtc();

    void reset();
    void tick_second();
    void write(std::uint8_t index, std::uint8_t value);
    [[nodiscard]] std::uint8_t read(std::uint8_t index) const;

private:
    std::array<std::uint8_t, 128> registers_{};
};

} // namespace cpm22::platform
