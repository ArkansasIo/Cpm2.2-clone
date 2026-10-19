#include "cmos_rtc.hpp"

#include <ctime>

namespace cpm22::platform {

CmosRtc::CmosRtc() { reset(); }

void CmosRtc::reset() {
    registers_.fill(0);
    registers_[0x0A] = 0x26; // RTC status A: divider enabled, nominal rate.
    registers_[0x0B] = 0x02; // 24-hour mode.
}

void CmosRtc::tick_second() {
    const auto now = std::time(nullptr);
    const auto* local = std::localtime(&now);
    if (!local) return;
    registers_[0x00] = static_cast<std::uint8_t>(local->tm_sec);
    registers_[0x02] = static_cast<std::uint8_t>(local->tm_min);
    registers_[0x04] = static_cast<std::uint8_t>(local->tm_hour);
    registers_[0x07] = static_cast<std::uint8_t>(local->tm_mday);
    registers_[0x08] = static_cast<std::uint8_t>(local->tm_mon + 1);
    registers_[0x09] = static_cast<std::uint8_t>((local->tm_year + 1900) % 100);
}

void CmosRtc::write(std::uint8_t index, std::uint8_t value) {
    registers_[index & 0x7f] = value;
}

std::uint8_t CmosRtc::read(std::uint8_t index) const {
    return registers_[index & 0x7f];
}

} // namespace cpm22::platform
