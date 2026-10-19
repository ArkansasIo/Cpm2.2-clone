#pragma once

#include <cstdint>
#include <string>

namespace cpm22::gui {

struct LinuxGuiConfig {
    std::uint32_t width = 1024;
    std::uint32_t height = 768;
    std::string title = "CP/M 2.2 Clone";
    bool fullscreen = false;
};

class LinuxGui {
public:
    explicit LinuxGui(LinuxGuiConfig config = {});
    bool initialize();
    void run_once();
    void shutdown();
    [[nodiscard]] bool running() const noexcept;
    [[nodiscard]] const LinuxGuiConfig& config() const noexcept;

private:
    LinuxGuiConfig config_;
    bool running_ = false;
};

} // namespace cpm22::gui
