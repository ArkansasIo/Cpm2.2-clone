#include "cpm/cpm_filesystem.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>

namespace cpm22::cpm {
namespace {
constexpr std::size_t kRecordSize = 128;
constexpr std::size_t kDirectoryEntrySize = 32;
constexpr std::size_t kRecordsPerExtent = 128;
constexpr std::uint8_t kUnused = 0xE5;

std::size_t extent_record_count(const CpmDirectoryRecord& entry) noexcept {
    if (entry.record_count != 0) return entry.record_count;
    for (const auto block : entry.allocation) {
        if (block != 0) return kRecordsPerExtent;
    }
    return 0;
}
}

CpmFilesystem::CpmFilesystem(DiskImage& disk) : disk_(disk) {}

std::size_t CpmFilesystem::directory_sector_count() const noexcept {
    return ((static_cast<std::size_t>(dpb().directory_max) + 1) * kDirectoryEntrySize + kRecordSize - 1) / kRecordSize;
}

std::size_t CpmFilesystem::data_start_sector() const noexcept {
    return static_cast<std::size_t>(dpb().track_offset) * dpb().sectors_per_track + directory_sector_count();
}

bool CpmFilesystem::format() {
    std::vector<std::uint8_t> blank(kRecordSize, kUnused);
    const auto start = static_cast<std::size_t>(dpb().track_offset) * dpb().sectors_per_track;
    for (std::size_t s = start; s < disk_.sector_count(); ++s) {
        if (!disk_.write_sector(s / dpb().sectors_per_track, s % dpb().sectors_per_track, blank)) return false;
    }
    return true;
}

bool CpmFilesystem::read_directory(std::vector<CpmDirectoryRecord>& entries) const {
    entries.clear();
    const std::size_t count = static_cast<std::size_t>(dpb().directory_max) + 1;
    for (std::size_t index = 0; index < count; ++index) {
        CpmDirectoryRecord entry;
        if (!load_entry(index, entry)) return false;
        entries.push_back(std::move(entry));
    }
    return true;
}

bool CpmFilesystem::load_entry(std::size_t index, CpmDirectoryRecord& entry) const {
    if (index > dpb().directory_max) return false;
    const std::size_t byte_offset = index * kDirectoryEntrySize;
    const std::size_t sector_offset = byte_offset / kRecordSize;
    const std::size_t offset = byte_offset % kRecordSize;
    std::vector<std::uint8_t> sector;
    const std::size_t absolute = static_cast<std::size_t>(dpb().track_offset) * dpb().sectors_per_track + sector_offset;
    if (!disk_.read_sector(absolute / dpb().sectors_per_track, absolute % dpb().sectors_per_track, sector)) return false;
    entry.user = sector[offset];
    entry.name.assign(reinterpret_cast<const char*>(sector.data() + offset + 1), 8);
    entry.extension.assign(reinterpret_cast<const char*>(sector.data() + offset + 9), 3);
    entry.extent = sector[offset + 12];
    entry.s1 = sector[offset + 13];
    entry.s2 = sector[offset + 14];
    entry.record_count = sector[offset + 15];
    entry.allocation.assign(sector.begin() + static_cast<std::ptrdiff_t>(offset + 16), sector.begin() + static_cast<std::ptrdiff_t>(offset + 32));
    return true;
}

bool CpmFilesystem::save_entry(std::size_t index, const CpmDirectoryRecord& entry) {
    if (index > dpb().directory_max || entry.allocation.size() != 16) return false;
    const std::size_t byte_offset = index * kDirectoryEntrySize;
    const std::size_t sector_offset = byte_offset / kRecordSize;
    const std::size_t offset = byte_offset % kRecordSize;
    std::vector<std::uint8_t> sector;
    const std::size_t absolute = static_cast<std::size_t>(dpb().track_offset) * dpb().sectors_per_track + sector_offset;
    if (!disk_.read_sector(absolute / dpb().sectors_per_track, absolute % dpb().sectors_per_track, sector)) return false;
    sector[offset] = entry.user;
    std::fill(sector.begin() + static_cast<std::ptrdiff_t>(offset + 1), sector.begin() + static_cast<std::ptrdiff_t>(offset + 12), ' ');
    for (std::size_t i = 0; i < std::min<std::size_t>(8, entry.name.size()); ++i) sector[offset + 1 + i] = static_cast<std::uint8_t>(std::toupper(static_cast<unsigned char>(entry.name[i])));
    std::fill(sector.begin() + static_cast<std::ptrdiff_t>(offset + 9), sector.begin() + static_cast<std::ptrdiff_t>(offset + 12), ' ');
    for (std::size_t i = 0; i < std::min<std::size_t>(3, entry.extension.size()); ++i) sector[offset + 9 + i] = static_cast<std::uint8_t>(std::toupper(static_cast<unsigned char>(entry.extension[i])));
    sector[offset + 12] = entry.extent;
    sector[offset + 13] = entry.s1;
    sector[offset + 14] = entry.s2;
    sector[offset + 15] = entry.record_count;
    std::copy(entry.allocation.begin(), entry.allocation.end(), sector.begin() + static_cast<std::ptrdiff_t>(offset + 16));
    return disk_.write_sector(absolute / dpb().sectors_per_track, absolute % dpb().sectors_per_track, sector);
}

bool CpmFilesystem::create(const Fcb& fcb) {
    std::vector<CpmDirectoryRecord> entries;
    if (!read_directory(entries)) return false;
    for (const auto& e : entries) if (e.user != kUnused && e.user == fcb.user && matches(fcb, e)) return false;
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].user == kUnused) {
            CpmDirectoryRecord e;
            e.user = fcb.user; e.name = fcb.filename();
            const auto dot = e.name.find('.');
            if (dot != std::string::npos) { e.extension = e.name.substr(dot + 1); e.name.resize(dot); }
            e.allocation.assign(16, 0);
            return save_entry(i, e);
        }
    }
    return false;
}

bool CpmFilesystem::open(const Fcb& fcb, Fcb* opened) const {
    std::vector<CpmDirectoryRecord> entries;
    if (!read_directory(entries)) return false;
    for (const auto& e : entries) {
        if (e.user != kUnused && e.user == fcb.user && matches(fcb, e)) {
            if (opened) *opened = to_fcb(e);
            return true;
        }
    }
    return false;
}

bool CpmFilesystem::remove(const Fcb& fcb) {
    std::vector<CpmDirectoryRecord> entries;
    if (!read_directory(entries)) return false;
    bool found = false;
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].user != kUnused && entries[i].user == fcb.user && matches(fcb, entries[i])) {
            if (!clear_entry(i)) return false;
            found = true;
        }
    }
    return found;
}

bool CpmFilesystem::rename(const Fcb& old_fcb, const Fcb& new_fcb) {
    std::vector<CpmDirectoryRecord> entries;
    if (!read_directory(entries)) return false;

    const auto replacement = new_fcb.filename();
    const auto dot = replacement.find('.');
    const auto base_name = replacement.substr(0, dot);
    const auto extension =
        dot == std::string::npos ? std::string{} : replacement.substr(dot + 1);

    // Enforce CP/M 8.3 bounds instead of writing oversized values into the
    // fixed-width directory entry.
    if (base_name.empty() || base_name.size() > 8 || extension.size() > 3) {
        return false;
    }

    // Locate the source first. A rename to the same canonical name is a
    // successful no-op; otherwise reject an existing destination.
    std::size_t source_index = entries.size();
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].user != kUnused && entries[i].user == old_fcb.user &&
            matches(old_fcb, entries[i])) {
            source_index = i;
            break;
        }
    }
    if (source_index == entries.size()) return false;

    for (std::size_t i = 0; i < entries.size(); ++i) {
        const auto& entry = entries[i];
        if (i != source_index && entry.user != kUnused &&
            entry.user == new_fcb.user && matches(new_fcb, entry)) {
            return false;
        }
    }

    auto& source = entries[source_index];
    source.name = base_name;
    source.extension = extension;
    return save_entry(source_index, source);
}

bool CpmFilesystem::clear_entry(std::size_t index) {
    CpmDirectoryRecord e;
    if (!load_entry(index, e)) return false;
    e.user = kUnused;
    std::fill(e.allocation.begin(), e.allocation.end(), 0);
    return save_entry(index, e);
}

std::size_t CpmFilesystem::extent_number(const CpmDirectoryRecord& entry) const noexcept {
    return static_cast<std::size_t>(entry.extent) + (static_cast<std::size_t>(entry.s2) << 5);
}

std::size_t CpmFilesystem::block_number(const CpmDirectoryRecord& entry, std::size_t allocation_index) const {
    return entry.allocation[allocation_index];
}

bool CpmFilesystem::block_reserved(std::size_t block) const noexcept {
    if (block >= 16) return false;
    return (dpb().directory_allocation & (std::uint16_t{1} << (15 - block))) != 0;
}

bool CpmFilesystem::block_used(std::size_t block) const {
    std::vector<CpmDirectoryRecord> entries;
    if (!read_directory(entries)) return true;
    for (const auto& e : entries) if (e.user != kUnused) for (auto b : e.allocation) if (b == block) return true;
    return false;
}

bool CpmFilesystem::allocate_block(std::size_t& block) const {
    for (std::size_t b = 0; b <= dpb().max_block; ++b) {
        if (!block_reserved(b) && !block_used(b)) { block = b; return true; }
    }
    return false;
}

std::size_t CpmFilesystem::free_block_count() const {
    std::size_t free = 0;
    for (std::size_t b = 0; b <= dpb().max_block; ++b) if (!block_reserved(b) && !block_used(b)) ++free;
    return free;
}

bool CpmFilesystem::read_record(const Fcb& fcb, std::size_t record, std::vector<std::uint8_t>& out) const {
    std::vector<CpmDirectoryRecord> entries;
    if (!read_directory(entries)) return false;
    const std::size_t target_extent = record / kRecordsPerExtent;
    const std::size_t within = record % kRecordsPerExtent;
    for (std::size_t i = 0; i < entries.size(); ++i) {
        const auto& e = entries[i];
        if (e.user == fcb.user && e.user != kUnused && matches(fcb, e) && extent_number(e) == target_extent) {
            if (within >= extent_record_count(e)) return false;
            return read_record_bytes(i, within, out);
        }
    }
    return false;
}

bool CpmFilesystem::write_record(const Fcb& fcb, std::size_t record, const std::vector<std::uint8_t>& data) {
    if (data.size() > kRecordSize) return false;
    std::vector<CpmDirectoryRecord> entries;
    if (!read_directory(entries)) return false;
    const std::size_t target_extent = record / kRecordsPerExtent;
    std::size_t index = entries.size();
    for (std::size_t i = 0; i < entries.size(); ++i) {
        if (entries[i].user != kUnused && entries[i].user == fcb.user && matches(fcb, entries[i]) && extent_number(entries[i]) == target_extent) {
            index = i;
            break;
        }
    }
    if (index == entries.size()) {
        for (std::size_t i = 0; i < entries.size(); ++i) {
            if (entries[i].user == kUnused) {
                CpmDirectoryRecord e;
                e.user = fcb.user;
                e.name = fcb.filename();
                const auto dot = e.name.find('.');
                if (dot != std::string::npos) {
                    e.extension = e.name.substr(dot + 1);
                    e.name.resize(dot);
                }
                e.extent = static_cast<std::uint8_t>(target_extent & 0xFFu);
                e.s2 = static_cast<std::uint8_t>((target_extent >> 5) & 0xFFu);
                e.allocation.assign(16, 0);
                if (!save_entry(i, e)) return false;
                index = i;
                break;
            }
        }
        if (index == entries.size()) return false;
        if (!read_directory(entries)) return false;
    }
    auto& e = entries[index];
    const std::size_t within = record % kRecordsPerExtent;
    const std::size_t records_per_block = dpb().block_size() / kRecordSize;
    if (records_per_block == 0) return false;
    const std::size_t block_slot = within / records_per_block;
    if (block_slot >= 16) return false;
    if (e.allocation[block_slot] == 0) {
        std::size_t block = 0;
        if (!allocate_block(block) || block > 0xFFu) return false;
        e.allocation[block_slot] = static_cast<std::uint8_t>(block);
        if (!save_entry(index, e)) return false;
    }
    if (!write_record_bytes(index, within, data)) return false;

    const std::size_t current_records = extent_record_count(e);
    const std::size_t new_records = std::max(current_records, within + 1);
    e.record_count = new_records >= kRecordsPerExtent ? 0 : static_cast<std::uint8_t>(new_records);
    return save_entry(index, e);
}

bool CpmFilesystem::read_record_bytes(std::size_t directory_index, std::size_t record, std::vector<std::uint8_t>& out) const {
    CpmDirectoryRecord e;
    if (!load_entry(directory_index, e)) return false;
    const std::size_t per_block = dpb().block_size() / kRecordSize;
    const std::size_t slot = record / per_block;
    if (slot >= 16 || e.allocation[slot] == 0) return false;
    const std::size_t sector = data_start_sector() + static_cast<std::size_t>(e.allocation[slot]) * per_block + (record % per_block);
    return disk_.read_sector(sector / dpb().sectors_per_track, sector % dpb().sectors_per_track, out);
}

bool CpmFilesystem::write_record_bytes(std::size_t directory_index, std::size_t record, const std::vector<std::uint8_t>& data) {
    CpmDirectoryRecord e;
    if (!load_entry(directory_index, e)) return false;
    const std::size_t per_block = dpb().block_size() / kRecordSize;
    const std::size_t slot = record / per_block;
    if (slot >= 16 || e.allocation[slot] == 0) return false;
    std::vector<std::uint8_t> sector(kRecordSize, 0x1A);
    std::copy(data.begin(), data.end(), sector.begin());
    const std::size_t absolute = data_start_sector() + static_cast<std::size_t>(e.allocation[slot]) * per_block + (record % per_block);
    return disk_.write_sector(absolute / dpb().sectors_per_track, absolute % dpb().sectors_per_track, sector);
}

bool CpmFilesystem::search(const Fcb& pattern, std::vector<Fcb>& results) const {
    results.clear();
    std::vector<CpmDirectoryRecord> entries;
    if (!read_directory(entries)) return false;
    for (const auto& e : entries) if (e.user != kUnused && e.user == pattern.user && matches(pattern, e)) results.push_back(to_fcb(e));
    return true;
}

bool CpmFilesystem::matches(const Fcb& pattern, const CpmDirectoryRecord& entry) {
    const auto candidate = to_fcb(entry);
    const auto field_matches = [](const auto& pattern_field, const auto& candidate_field) {
        std::size_t i = 0;
        while (i < pattern_field.size()) {
            const char p = pattern_field[i];
            const char c = candidate_field[i];

            if (p == '*') {
                // '*' matches zero or more characters in the remaining field.
                return true;
            }
            if (p == '?') {
                if (c == ' ' || c == '\\0') return false;
                ++i;
                continue;
            }
            if (p == ' ' || p == '\\0') {
                // The remaining pattern is blank padding. The candidate must
                // also contain only padding from this position onward.
                for (std::size_t j = i; j < candidate_field.size(); ++j) {
                    if (candidate_field[j] != ' ' && candidate_field[j] != '\\0') return false;
                }
                return true;
            }
            if (std::toupper(static_cast<unsigned char>(p)) !=
                std::toupper(static_cast<unsigned char>(c))) {
                return false;
            }
            ++i;
        }
        return true;
    };

    return field_matches(pattern.name, candidate.name) &&
           field_matches(pattern.extension, candidate.extension);
}

Fcb CpmFilesystem::to_fcb(const CpmDirectoryRecord& entry) {
    Fcb f;
    f.user = entry.user;
    // The CP/M on-disk entry stores eight name bytes and three extension
    // bytes without a terminator. Convert them into canonical space-padded
    // FCB fields before exposing them to matching and API callers.
    f.name.fill(' ');
    f.extension.fill(' ');
    for (std::size_t i = 0; i < f.name.size() && i < entry.name.size(); ++i) {
        f.name[i] = entry.name[i];
    }
    for (std::size_t i = 0; i < f.extension.size() && i < entry.extension.size(); ++i) {
        f.extension[i] = entry.extension[i];
    }
    f.extent = entry.extent;
    f.record_count = entry.record_count;
    return f;
}

void CpmFilesystem::normalize_name(std::string& name, std::size_t width) {
    for (auto& c : name) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    if (name.size() > width) name.resize(width);
}

} // namespace cpm22::cpm
