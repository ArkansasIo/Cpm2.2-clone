#pragma once

#include "cpm/dpb.hpp"
#include "devices/storage.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace cpm22::cpm {

class DiskImage {
public:
    explicit DiskImage(DiskParameterBlock dpb = {}, std::size_t tracks = 77);

    bool read_sector(std::size_t track, std::size_t sector, std::vector<std::uint8_t>& out);
    bool write_sector(std::size_t track, std::size_t sector, const std::vector<std::uint8_t>& data);
    bool save(const std::string& path) const;
    bool load(const std::string& path);

    [[nodiscard]] const DiskParameterBlock& dpb() const noexcept { return dpb_; }
    [[nodiscard]] std::size_t sector_size() const noexcept { return disk_.sector_size(); }
    [[nodiscard]] std::size_t sector_count() const noexcept { return disk_.sector_count(); }

private:
    DiskParameterBlock dpb_;
    devices::VirtualDisk disk_;
};

} // namespace cpm22::cpm
