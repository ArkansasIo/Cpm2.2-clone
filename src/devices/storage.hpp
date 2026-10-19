#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace cpm22::devices {

enum class StorageOperation { Read, Write, Seek, Load, Save, Create };

struct StorageEvent {
    StorageOperation operation;
    std::uint64_t sector;
    std::size_t bytes;
};

class IStorageObserver {
public:
    virtual ~IStorageObserver() = default;
    virtual void on_storage_event(const StorageEvent& event) = 0;
};

class VirtualDisk {
public:
    explicit VirtualDisk(std::size_t sectors = 256, std::size_t sector_size = 128);

    void add_observer(IStorageObserver* observer);
    [[nodiscard]] std::size_t sector_size() const noexcept;
    [[nodiscard]] std::size_t sector_count() const noexcept;

    bool read(std::size_t sector, std::vector<std::uint8_t>& out) const;
    bool write(std::size_t sector, const std::vector<std::uint8_t>& data);

private:
    std::size_t sector_size_;
    std::vector<std::uint8_t> data_;
    std::vector<IStorageObserver*> observers_;

    void notify(StorageOperation operation, std::size_t sector, std::size_t bytes);
};

} // namespace cpm22::devices
