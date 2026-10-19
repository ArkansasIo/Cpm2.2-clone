#include "cpm/disk_image.hpp"

#include <fstream>

namespace cpm22::cpm {

DiskImage::DiskImage(DiskParameterBlock dpb, std::size_t tracks)
    : dpb_(dpb), disk_(tracks * dpb_.sectors_per_track, 128) {}

bool DiskImage::read_sector(std::size_t track, std::size_t sector,
                            std::vector<std::uint8_t>& out) {
    if (track * dpb_.sectors_per_track + sector >= disk_.sector_count()) return false;
    if (sector >= dpb_.sectors_per_track) return false;
    return disk_.read(track * dpb_.sectors_per_track + sector, out);
}

bool DiskImage::write_sector(std::size_t track, std::size_t sector,
                             const std::vector<std::uint8_t>& data) {
    if (track * dpb_.sectors_per_track + sector >= disk_.sector_count()) return false;
    if (sector >= dpb_.sectors_per_track) return false;
    return disk_.write(track * dpb_.sectors_per_track + sector, data);
}

bool DiskImage::save(const std::string& path) const {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;

    std::vector<std::uint8_t> sector;
    for (std::size_t i = 0; i < disk_.sector_count(); ++i) {
        if (!disk_.read(i, sector)) return false;
        file.write(reinterpret_cast<const char*>(sector.data()),
                   static_cast<std::streamsize>(sector.size()));
        if (!file) return false;
    }
    return true;
}

bool DiskImage::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;

    std::vector<std::uint8_t> sector(disk_.sector_size());
    for (std::size_t i = 0; i < disk_.sector_count(); ++i) {
        file.read(reinterpret_cast<char*>(sector.data()),
                  static_cast<std::streamsize>(sector.size()));
        if (file.gcount() != static_cast<std::streamsize>(sector.size())) return false;
        if (!disk_.write(i, sector)) return false;
    }
    return true;
}

} // namespace cpm22::cpm
