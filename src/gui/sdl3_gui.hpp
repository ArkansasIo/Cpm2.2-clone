#pragma once

#include "linux_gui.hpp"

namespace cpm22::gui {

class Sdl3Gui {
public:
    explicit Sdl3Gui(LinuxGuiConfig config = {});
    ~Sdl3Gui();

    Sdl3Gui(const Sdl3Gui&) = delete;
    Sdl3Gui& operator=(const Sdl3Gui&) = delete;

    bool initialize();
    void run_once();
    void shutdown();
    [[nodiscard]] bool running() const noexcept;
    [[nodiscard]] bool backend_available() const noexcept;

private:
    LinuxGuiConfig config_;
    bool running_ = false;
    bool backend_available_ = false;
    void* window_ = nullptr;
    void* renderer_ = nullptr;
};

} // namespace cpm22::gui
