#include "cpm/bdos.hpp"

#include <algorithm>
#include <utility>

namespace cpm22::cpm {

namespace {
constexpr std::size_t kRecordSize = 128;
constexpr std::uint8_t kOk = 0;
constexpr std::uint8_t kError = 0xff;
}

Bdos::Bdos(memory::Memory& memory) : memory_(memory) {}

void Bdos::set_console(ConsoleInput input, ConsoleOutput output) {
    input_ = std::move(input);
    output_ = std::move(output);
}

Fcb Bdos::read_fcb(std::uint16_t address) const {
    Fcb f;
    f.user = memory_.read(address);
    for (std::size_t i = 0; i < f.name.size(); ++i) f.name[i] = static_cast<char>(memory_.read(address + 1 + i));
    for (std::size_t i = 0; i < f.extension.size(); ++i) f.extension[i] = static_cast<char>(memory_.read(address + 9 + i));
    f.extent = memory_.read(address + 12);
    f.reserved = memory_.read(address + 13);
    f.record_count = memory_.read(address + 15);
    for (std::size_t i = 0; i < f.allocation.size(); ++i) f.allocation[i] = memory_.read(address + 16 + i);
    return f;
}

void Bdos::write_fcb(std::uint16_t address, const Fcb& fcb) {
    memory_.write(address, fcb.user);
    for (std::size_t i = 0; i < fcb.name.size(); ++i) memory_.write(address + 1 + i, static_cast<std::uint8_t>(fcb.name[i]));
    for (std::size_t i = 0; i < fcb.extension.size(); ++i) memory_.write(address + 9 + i, static_cast<std::uint8_t>(fcb.extension[i]));
    memory_.write(address + 12, fcb.extent);
    memory_.write(address + 13, fcb.reserved);
    memory_.write(address + 15, fcb.record_count);
    for (std::size_t i = 0; i < fcb.allocation.size(); ++i) memory_.write(address + 16 + i, fcb.allocation[i]);
}

std::string Bdos::key(const Fcb& fcb) const {
    return std::to_string(fcb.user == 0 ? current_user_ : fcb.user) + ":" + fcb.filename();
}

void Bdos::sync_directory_entry(const Fcb& fcb, std::size_t bytes) {
    const auto filename = fcb.filename();
    if (directory_.contains(filename, fcb.user)) return;
    DirectoryEntry entry;
    entry.user = fcb.user;
    const auto dot = filename.find('.');
    entry.name = filename.substr(0, dot);
    entry.extension = dot == std::string::npos ? "" : filename.substr(dot + 1);
    entry.size_bytes = bytes;
    directory_.add(entry);
}

std::uint8_t Bdos::search(const Fcb& fcb, bool first) {
    if (first) {
        search_results_.clear();
        for (const auto& entry : directory_.list(fcb.user)) {
            const auto name = entry.extension.empty() ? entry.name : entry.name + "." + entry.extension;
            if (fcb.filename() == "*.*" || fcb.filename() == name) search_results_.push_back(name);
        }
        search_index_ = 0;
    }
    if (search_index_ >= search_results_.size()) return kError;
    write_fcb(dma_, Fcb::from_string(search_results_[search_index_++], fcb.user));
    return kOk;
}

std::uint8_t Bdos::dispatch(std::uint8_t function, std::uint16_t de) {
    last_error_.clear();
    switch (function) {
    case 2:
        if (output_) output_(static_cast<char>(de & 0xff));
        return kOk;
    case 6:
        if (de == 0xff) return input_ ? static_cast<std::uint8_t>(std::max(input_(), 0)) : 0;
        if (output_) output_(static_cast<char>(de & 0xff));
        return kOk;
    case 9:
        for (std::uint16_t p = de;; ++p) { const auto ch = memory_.read(p); if (ch == '$') break; if (output_) output_(static_cast<char>(ch)); }
        return kOk;
    case 11: return 0;
    case 12: return 0x22;
    case 13: return kOk;
    case 14: current_disk_ = static_cast<std::uint8_t>(de & 0xff); return current_disk_ == 0 ? kOk : kError;
    case 15: return files_.contains(key(read_fcb(de))) ? kOk : kError;
    case 16: return files_.contains(key(read_fcb(de))) ? kOk : kError;
    case 17: return search(read_fcb(de), true);
    case 18: return search(read_fcb(de), false);
    case 19: { const auto f = read_fcb(de); if (files_.erase(key(f)) == 0) return kError; directory_.remove(f.filename(), f.user); return kOk; }
    case 20: {
        const auto f = read_fcb(de); const auto it = files_.find(key(f)); if (it == files_.end()) return kError;
        const auto offset = (static_cast<std::size_t>(f.extent) * 128 + f.record_count) * kRecordSize;
        if (offset >= it->second.size()) return 1;
        const auto count = std::min(kRecordSize, it->second.size() - offset);
        for (std::size_t i = 0; i < kRecordSize; ++i) memory_.write(dma_ + static_cast<std::uint16_t>(i), i < count ? it->second[offset + i] : 0x1a);
        return kOk;
    }
    case 21: {
        auto f = read_fcb(de); auto& data = files_[key(f)]; const auto offset = (static_cast<std::size_t>(f.extent) * 128 + f.record_count) * kRecordSize;
        if (data.size() < offset + kRecordSize) data.resize(offset + kRecordSize, 0x1a);
        for (std::size_t i = 0; i < kRecordSize; ++i) data[offset + i] = memory_.read(dma_ + static_cast<std::uint16_t>(i));
        ++f.record_count; write_fcb(de, f); sync_directory_entry(f, data.size()); return kOk;
    }
    case 22: { const auto f = read_fcb(de); if (files_.contains(key(f))) return kError; files_[key(f)] = {}; sync_directory_entry(f, 0); return kOk; }
    case 23: return kError; // Rename requires the second FCB view; left explicit until directory metadata supports atomic rename.
    case 24: return kOk;
    case 25: return current_disk_;
    case 26: dma_ = de; return kOk;
    case 32: if ((de & 0xff) == 0xff) return current_user_; current_user_ = static_cast<std::uint8_t>(de & 0x0f); return current_user_;
    default: last_error_ = "Unsupported BDOS function " + std::to_string(function); return kError;
    }
}

} // namespace cpm22::cpm
