#pragma once

#include <cstddef>
#include <cstdint>

namespace cpm22::platform {

// The guest CPU/application model is deliberately 8-bit. The host/OS layer may
// be 64-bit and can expose larger memory/address-space services to the guest.
struct ArchitectureConfig {
    std::uint8_t guest_word_bits = 8;
    std::uint8_t guest_address_bits = 16;
    std::uint8_t host_word_bits = 64;
    std::size_t host_memory_bytes = 16ull * 1024ull * 1024ull;
};

class ArchitectureLayer {
public:
    explicit ArchitectureLayer(ArchitectureConfig config = {});

    [[nodiscard]] const ArchitectureConfig& config() const noexcept;
    [[nodiscard]] bool is_8bit_guest() const noexcept;
    [[nodiscard]] bool is_64bit_host() const noexcept;

private:
    ArchitectureConfig config_;
};

} // namespace cpm22::platform
