#include "cpm/directory.hpp"
#include <algorithm>
#include <cctype>

namespace cpm22::cpm {
namespace {
std::string normalize(std::string value) {
    for (char& ch : value) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    return value;
}
}

bool Directory::add(DirectoryEntry entry) {
    entry.name = normalize(entry.name);
    entry.extension = normalize(entry.extension);
    if (entry.name.empty() || entry.name.size() > 8 || entry.extension.size() > 3 || contains(entry.name, entry.extension)) return false;
    entries_.push_back(std::move(entry));
    return true;
}

bool Directory::remove(const std::string& name, const std::string& extension) {
    const auto n = normalize(name);
    const auto e = normalize(extension);
    const auto old_size = entries_.size();
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(), [&](const DirectoryEntry& x) {
        return x.name == n && x.extension == e;
    }), entries_.end());
    return entries_.size() != old_size;
}

std::vector<DirectoryEntry> Directory::list(std::uint8_t user) const {
    std::vector<DirectoryEntry> result;
    for (const auto& entry : entries_) if (entry.user == user) result.push_back(entry);
    return result;
}

bool Directory::contains(const std::string& name, const std::string& extension) const {
    const auto n = normalize(name);
    const auto e = normalize(extension);
    return std::any_of(entries_.begin(), entries_.end(), [&](const DirectoryEntry& x) { return x.name == n && x.extension == e; });
}

} // namespace cpm22::cpm
