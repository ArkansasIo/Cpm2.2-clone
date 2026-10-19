#include "linux_gui.hpp"

namespace cpm22::gui {

LinuxGui::LinuxGui(LinuxGuiConfig config) : config_(std::move(config)) {}

bool LinuxGui::initialize() {
    running_ = true;
    return true;
}

void LinuxGui::run_once() {
    if (!running_) return;
    // Backend-neutral event loop hook. A concrete SDL2/Wayland/X11 backend
    // can call this from the platform frontend without changing the emulator.
}

void LinuxGui::shutdown() { running_ = false; }

bool LinuxGui::running() const noexcept { return running_; }

const LinuxGuiConfig& LinuxGui::config() const noexcept { return config_; }

} // namespace cpm22::gui
