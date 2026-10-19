#include "retro_audio.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <random>

namespace cpm22::audio {
namespace {

constexpr double kPi = 3.14159265358979323846;

std::string pattern_for(StorageEvent event) {
    switch (event) {
    case StorageEvent::Read:    return ".";
    case StorageEvent::Write:   return "-";
    case StorageEvent::Save:    return "--.";
    case StorageEvent::Load:    return "-..";
    case StorageEvent::Create:  return "..-";
    case StorageEvent::Seek:    return ".-";
    case StorageEvent::Printer: return ". .";
    case StorageEvent::Error:   return "---";
    }
    return ".";
}

void write_u16(std::ofstream& out, std::uint16_t v) {
    out.put(static_cast<char>(v & 0xff));
    out.put(static_cast<char>((v >> 8) & 0xff));
}

void write_u32(std::ofstream& out, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) out.put(static_cast<char>((v >> (i * 8)) & 0xff));
}

} // namespace

RetroAudioEngine::RetroAudioEngine(AudioSettings settings) : settings_(settings) {}

void RetroAudioEngine::set_settings(const AudioSettings& settings) { settings_ = settings; }

const AudioSettings& RetroAudioEngine::settings() const noexcept { return settings_; }

void RetroAudioEngine::on_storage_event(StorageEvent event) {
    emit_morse(pattern_for(event));
}

void RetroAudioEngine::emit_morse(const std::string& pattern) {
    patterns_.push_back(pattern);
}

std::vector<std::int8_t> RetroAudioEngine::render_pcm() const {
    constexpr int sample_rate = 44100;
    std::vector<std::int8_t> pcm;
    std::mt19937 rng(0xC0FFEEu);
    std::uniform_real_distribution<double> noise(-1.0, 1.0);

    auto append_silence = [&](double seconds) {
        pcm.insert(pcm.end(), static_cast<std::size_t>(seconds * sample_rate), 0);
    };

    for (const auto& pattern : patterns_) {
        for (const char symbol : pattern) {
            if (symbol == ' ') {
                append_silence(settings_.gap_seconds);
                continue;
            }
            const double duration = symbol == '-' ? settings_.dash_seconds : settings_.dot_seconds;
            const auto count = static_cast<std::size_t>(duration * sample_rate);
            for (std::size_t i = 0; i < count; ++i) {
                const double t = static_cast<double>(i) / sample_rate;
                const double carrier = std::sin(2.0 * kPi * settings_.frequency_hz * t);
                const double n = noise(rng) * settings_.noise_amount;
                const double sample = std::clamp((carrier * (1.0 - settings_.noise_amount) + n) * settings_.volume, -1.0, 1.0);
                pcm.push_back(static_cast<std::int8_t>(sample * 127.0));
            }
            append_silence(settings_.gap_seconds);
        }
    }
    return pcm;
}

bool RetroAudioEngine::write_wav(const std::string& path, int sample_rate) const {
    const auto pcm = render_pcm();
    std::ofstream out(path, std::ios::binary);
    if (!out) return false;

    const std::uint32_t data_size = static_cast<std::uint32_t>(pcm.size());
    const std::uint32_t riff_size = 36u + data_size;
    out.write("RIFF", 4);
    write_u32(out, riff_size);
    out.write("WAVEfmt ", 8);
    write_u32(out, 16);
    write_u16(out, 1);
    write_u16(out, 1);
    write_u32(out, static_cast<std::uint32_t>(sample_rate));
    write_u32(out, static_cast<std::uint32_t>(sample_rate));
    write_u16(out, 1);
    write_u16(out, 8);
    out.write("data", 4);
    write_u32(out, data_size);
    for (const auto sample : pcm) out.put(static_cast<char>(static_cast<std::uint8_t>(sample)));
    return static_cast<bool>(out);
}

} // namespace cpm22::audio
