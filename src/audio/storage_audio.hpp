#pragma once

#include "retro_audio.hpp"
#include "../devices/storage.hpp"

namespace cpm22::audio {

class StorageAudioBridge final : public devices::IStorageObserver {
public:
    explicit StorageAudioBridge(RetroAudioEngine& engine) : engine_(engine) {}
    void on_storage_event(const devices::StorageEvent& event) override;

private:
    RetroAudioEngine& engine_;
};

} // namespace cpm22::audio
