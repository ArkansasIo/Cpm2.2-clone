#pragma once

#include "cpm/directory.hpp"
#include "cpm/fcb.hpp"
#include "memory/memory.hpp"
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace cpm22::cpm {

class Bdos {
public:
    using ConsoleInput = std::function<int()>;
    using ConsoleOutput = std::function<void(char)>;

    explicit Bdos(memory::Memory& memory);
    void set_console(ConsoleInput input, ConsoleOutput output);
    std::uint8_t dispatch(std::uint8_t function, std::uint16_t de);
    [[nodiscard]] Directory& directory() noexcept { return directory_; }
    [[nodiscard]] const std::string& last_error() const noexcept { return last_error_; }

private:
    using FileStore = std::unordered_map<std::string, std::vector<std::uint8_t>>;

    memory::Memory& memory_;
    Directory directory_;
    ConsoleInput input_;
    ConsoleOutput output_;
    std::string last_error_;
    FileStore files_;
    std::uint8_t current_disk_ = 0;
    std::uint8_t current_user_ = 0;
    std::uint16_t dma_ = 0x0080;
    std::string search_pattern_;
    std::vector<std::string> search_results_;
    std::size_t search_index_ = 0;

    Fcb read_fcb(std::uint16_t address) const;
    void write_fcb(std::uint16_t address, const Fcb& fcb);
    std::string key(const Fcb& fcb) const;
    void sync_directory_entry(const Fcb& fcb, std::size_t bytes);
    std::uint8_t search(const Fcb& fcb, bool first);
};

} // namespace cpm22::cpm
