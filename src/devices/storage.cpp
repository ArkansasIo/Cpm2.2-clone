#include "storage.hpp"

#include <algorithm>

namespace cpm22::devices {

VirtualDisk::VirtualDisk(std::size_t sectors, std::size_t sector_size)
    : sector_size_(sector_size), data_(sectors * sector_size, 0) {}

void VirtualDisk::add_observer(IStorageObserver* observer) {
    if (observer && std::find(observers_.begin(), observers_.end(), observer) == observers_.end())
        observers_.push_back(observer);
}

std::size_t VirtualDisk::sector_size() const noexcept { return sector_size_; }
std::size_t VirtualDisk::sector_count() const noexcept { return data_.size() / sector_size_; }

bool VirtualDisk::read(std::size_t sector, std::vector<std::uint8_t>& out) const {
    if (sector >= sector_count()) return false;
    const auto begin = data_.begin() + static_cast<std::ptrdiff_t>(sector * sector_size_);
    out.assign(begin, begin + static_cast<std::ptrdiff_t>(sector_size_));
    return true;
}

bool VirtualDisk::write(std::size_t sector, const std::vector<std::uint8_t>& data) {
    if (sector >= sector_count() || data.size() > sector_size_) return false;
    const auto begin = data_.begin() + static_cast<std::ptrdiff_t>(sector * sector_size_);
    std::copy(data.begin(), data.end(), begin);
    std::fill(begin + static_cast<std::ptrdiff_t>(data.size()),
              begin + static_cast<std::ptrdiff_t>(sector_size_), 0);
    notify(StorageOperation::Seek, sector, 0);
    notify(StorageOperation::Write, sector, sector_size_);
    notify(StorageOperation::Save, sector, sector_size_);
    return true;
}

void VirtualDisk::notify(StorageOperation operation, std::size_t sector, std::size_t bytes) {
    const StorageEvent event{operation, sector, bytes};
    for (auto* observer : observers_) observer->on_storage_event(event);
}

} // namespace cpm22::devices
