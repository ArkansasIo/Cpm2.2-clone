#pragma once

#include <cstdint>
#include <string>

namespace cpm22::platform {

struct LinuxKernelOptions {
    bool enabled = false;
    std::uint64_t memory_bytes = 16ull * 1024ull * 1024ull;
    std::string command_line = "console=ttyS0";
};

class LinuxKernelLayer {
public:
    explicit LinuxKernelLayer(LinuxKernelOptions options = {});

    void configure(LinuxKernelOptions options);
    [[nodiscard]] const LinuxKernelOptions& options() const noexcept;
    [[nodiscard]] bool can_boot() const noexcept;

private:
    LinuxKernelOptions options_;
};

} // namespace cpm22::platform
