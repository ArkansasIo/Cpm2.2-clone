#include "sdl3_gui.hpp"

#ifdef CPM22_ENABLE_SDL3
#include <SDL3/SDL.h>
#endif

#include <utility>

namespace cpm22::gui {

Sdl3Gui::Sdl3Gui(LinuxGuiConfig config) : config_(std::move(config)) {}

Sdl3Gui::~Sdl3Gui() { shutdown(); }

bool Sdl3Gui::initialize() {
#ifdef CPM22_ENABLE_SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) return false;

    const auto flags = config_.fullscreen ? SDL_WINDOW_FULLSCREEN : 0u;
    window_ = SDL_CreateWindow(config_.title.c_str(),
                               static_cast<int>(config_.width),
                               static_cast<int>(config_.height), flags);
    if (!window_) {
        SDL_Quit();
        return false;
    }

    renderer_ = SDL_CreateRenderer(static_cast<SDL_Window*>(window_), nullptr);
    if (!renderer_) {
        SDL_DestroyWindow(static_cast<SDL_Window*>(window_));
        window_ = nullptr;
        SDL_Quit();
        return false;
    }

    backend_available_ = true;
#else
    backend_available_ = false;
#endif
    running_ = true;
    return true;
}

void Sdl3Gui::run_once() {
    if (!running_) return;
#ifdef CPM22_ENABLE_SDL3
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) running_ = false;
    }
    if (renderer_) {
        SDL_SetRenderDrawColor(static_cast<SDL_Renderer*>(renderer_), 0, 12, 18, 255);
        SDL_RenderClear(static_cast<SDL_Renderer*>(renderer_));
        SDL_RenderPresent(static_cast<SDL_Renderer*>(renderer_));
    }
#endif
}

void Sdl3Gui::shutdown() {
#ifdef CPM22_ENABLE_SDL3
    if (renderer_) {
        SDL_DestroyRenderer(static_cast<SDL_Renderer*>(renderer_));
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(static_cast<SDL_Window*>(window_));
        window_ = nullptr;
    }
    if (backend_available_) SDL_Quit();
#endif
    running_ = false;
}

bool Sdl3Gui::running() const noexcept { return running_; }
bool Sdl3Gui::backend_available() const noexcept { return backend_available_; }

} // namespace cpm22::gui
