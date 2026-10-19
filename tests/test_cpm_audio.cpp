#include "audio/retro_audio.hpp"
#include "audio/storage_audio.hpp"
#include "devices/storage.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <vector>

using namespace cpm22;

int main() {
    audio::RetroAudioEngine audio_engine;
    devices::VirtualDisk disk(64, 128);
    audio::StorageAudioBridge bridge(audio_engine);
    disk.add_observer(&bridge);

    std::vector<std::uint8_t> write_buffer(128);
    for (std::size_t i = 0; i < write_buffer.size(); ++i)
        write_buffer[i] = static_cast<std::uint8_t>(i);

    assert(disk.write(0, write_buffer));

    std::vector<std::uint8_t> read_buffer;
    assert(disk.read(0, read_buffer));
    assert(read_buffer == write_buffer);

    // Reject out-of-range sectors and payloads that exceed a sector.
    assert(!disk.read(disk.sector_count(), read_buffer));
    assert(!disk.write(disk.sector_count(), write_buffer));
    assert(!disk.write(1, std::vector<std::uint8_t>(disk.sector_size() + 1, 0)));

    audio_engine.on_storage_event(audio::StorageEvent::Create);
    audio_engine.on_storage_event(audio::StorageEvent::Load);
    audio_engine.on_storage_event(audio::StorageEvent::Save);

    const auto pcm = audio_engine.render_pcm();
    assert(!pcm.empty());

    const auto path = std::filesystem::temp_directory_path() / "cpm22_storage_audio_test.wav";
    assert(audio_engine.write_wav(path.string(), 44100));
    assert(std::filesystem::exists(path));
    assert(std::filesystem::file_size(path) > 44);
    std::filesystem::remove(path);

    std::cout << "CP/M storage/audio integration test passed\n";
    return 0;
}
