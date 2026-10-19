#include "console.hpp"
namespace cpm22::cpm { void Console::write(std::string_view t){ output_.append(t); } void Console::writeln(std::string_view t){ output_.append(t); output_.push_back('\n'); } }
