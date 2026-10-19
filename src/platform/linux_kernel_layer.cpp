#include "linux_kernel_layer.hpp"

namespace cpm22::platform {

LinuxKernelLayer::LinuxKernelLayer(LinuxKernelOptions options) : options_(std::move(options)) {}

void LinuxKernelLayer::configure(LinuxKernelOptions options) { options_ = std::move(options); }

const LinuxKernelOptions& LinuxKernelLayer::options() const noexcept { return options_; }

bool LinuxKernelLayer::can_boot() const noexcept {
    // This layer models the boot contract; a real Linux kernel image is supplied separately.
    return options_.enabled && options_.memory_bytes >= (2ull * 1024ull * 1024ull);
}

} // namespace cpm22::platform
