#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace cpm22::audio {

enum class StorageEvent {
    Read,
    Write,
    Save,
    Load,
    Create,
    Seek,
    Printer,
    Error
};

struct AudioSettings {
    double frequency_hz = 900.0;
    double noise_amount = 0.18;
    double volume = 0.75;
    double dot_seconds = 0.055;
    double dash_seconds = 0.165;
    double gap_seconds = 0.055;
};

class RetroAudioEngine {
public:
    explicit RetroAudioEngine(AudioSettings settings = {});

    void set_settings(const AudioSettings& settings);
    [[nodiscard]] const AudioSettings& settings() const noexcept;

    // These methods are intended to be called by real CP/M/BIOS storage events.
    void on_storage_event(StorageEvent event);
    void emit_morse(const std::string& pattern);

    // Render the accumulated event stream as signed 8-bit mono PCM.
    [[nodiscard]] std::vector<std::int8_t> render_pcm() const;
    [[nodiscard]] bool write_wav(const std::string& path, int sample_rate = 44100) const;

private:
    AudioSettings settings_;
    std::vector<std::string> patterns_;
};

} // namespace cpm22::audio
