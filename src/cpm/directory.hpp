#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace cpm22::cpm {

struct DirectoryEntry {
    std::uint8_t user = 0;
    std::string name;
    std::string extension;
    std::uint32_t size_bytes = 0;
    bool read_only = false;
};

class Directory {
public:
    bool add(DirectoryEntry entry);
    bool remove(const std::string& name, const std::string& extension);
    [[nodiscard]] std::vector<DirectoryEntry> list(std::uint8_t user = 0) const;
    [[nodiscard]] bool contains(const std::string& name, const std::string& extension) const;

private:
    std::vector<DirectoryEntry> entries_;
};

} // namespace cpm22::cpm
