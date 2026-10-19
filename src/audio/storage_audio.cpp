#include "storage_audio.hpp"

namespace cpm22::audio {

void StorageAudioBridge::on_storage_event(const devices::StorageEvent& event) {
    switch (event.operation) {
    case devices::StorageOperation::Read:   engine_.on_storage_event(StorageEvent::Read); break;
    case devices::StorageOperation::Write:  engine_.on_storage_event(StorageEvent::Write); break;
    case devices::StorageOperation::Seek:   engine_.on_storage_event(StorageEvent::Seek); break;
    case devices::StorageOperation::Load:   engine_.on_storage_event(StorageEvent::Load); break;
    case devices::StorageOperation::Save:   engine_.on_storage_event(StorageEvent::Save); break;
    case devices::StorageOperation::Create: engine_.on_storage_event(StorageEvent::Create); break;
    }
}

} // namespace cpm22::audio
