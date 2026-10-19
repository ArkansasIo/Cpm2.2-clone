#pragma once

#include "cpm/disk_image.hpp"
#include "cpm/fcb.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace cpm22::cpm {

struct CpmDirectoryRecord {
    std::uint8_t user = 0xE5;
    std::string name;
    std::string extension;
    std::uint8_t extent = 0;
    std::uint8_t s1 = 0;
    std::uint8_t s2 = 0;
    std::uint8_t record_count = 0;
    std::vector<std::uint8_t> allocation;
};

class CpmFilesystem {
public:
    explicit CpmFilesystem(DiskImage& disk);

    bool format();
    bool create(const Fcb& fcb);
    bool open(const Fcb& fcb, Fcb* opened = nullptr) const;
    bool remove(const Fcb& fcb);
    bool rename(const Fcb& old_fcb, const Fcb& new_fcb);
    bool read_record(const Fcb& fcb, std::size_t record, std::vector<std::uint8_t>& out) const;
    bool write_record(const Fcb& fcb, std::size_t record, const std::vector<std::uint8_t>& data);
    bool search(const Fcb& pattern, std::vector<Fcb>& results) const;
    [[nodiscard]] std::size_t free_block_count() const;

private:
    DiskImage& disk_;
    const DiskParameterBlock& dpb() const noexcept { return disk_.dpb(); }
    std::size_t directory_sector_count() const noexcept;
    std::size_t data_start_sector() const noexcept;
    bool read_directory(std::vector<CpmDirectoryRecord>& entries) const;
    bool write_directory(const std::vector<CpmDirectoryRecord>& entries);
    bool read_record_bytes(std::size_t directory_index, std::size_t record, std::vector<std::uint8_t>& out) const;
    bool write_record_bytes(std::size_t directory_index, std::size_t record, const std::vector<std::uint8_t>& data);
    bool load_entry(std::size_t index, CpmDirectoryRecord& entry) const;
    bool save_entry(std::size_t index, const CpmDirectoryRecord& entry);
    std::size_t extent_number(const CpmDirectoryRecord& entry) const noexcept;
    std::size_t block_number(const CpmDirectoryRecord& entry, std::size_t allocation_index) const;
    bool block_reserved(std::size_t block) const noexcept;
    bool block_used(std::size_t block) const;
    bool allocate_block(std::size_t& block) const;
    bool clear_entry(std::size_t index);
    static bool matches(const Fcb& pattern, const CpmDirectoryRecord& entry);
    static Fcb to_fcb(const CpmDirectoryRecord& entry);
    static void normalize_name(std::string& name, std::size_t width);
};

} // namespace cpm22::cpm
