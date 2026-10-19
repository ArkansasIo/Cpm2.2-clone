#include "architecture.hpp"

namespace cpm22::platform {

ArchitectureLayer::ArchitectureLayer(ArchitectureConfig config) : config_(config) {}

const ArchitectureConfig& ArchitectureLayer::config() const noexcept { return config_; }

bool ArchitectureLayer::is_8bit_guest() const noexcept { return config_.guest_word_bits == 8; }

bool ArchitectureLayer::is_64bit_host() const noexcept { return config_.host_word_bits == 64; }

} // namespace cpm22::platform
