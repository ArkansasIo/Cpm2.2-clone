#pragma once
#include <string>
#include <string_view>
namespace cpm22::cpm {
class Console {
public:
    void write(std::string_view text);
    void writeln(std::string_view text);
    [[nodiscard]] const std::string& output() const noexcept { return output_; }
    void clear() noexcept { output_.clear(); }
private: std::string output_;
};
}
