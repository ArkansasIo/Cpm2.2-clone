#include "cpm/cpm_filesystem.hpp"
#include <cassert>
#include <string>
#include <vector>

int main() {
    cpm22::cpm::DiskImage disk;
    cpm22::cpm::CpmFilesystem fs(disk);
    assert(fs.format());

    const auto fcb = cpm22::cpm::Fcb::from_string("HELLO.TXT", 0);
    assert(fs.create(fcb));
    assert(fs.open(fcb));

    std::vector<std::uint8_t> write(128, 0x41);
    assert(fs.write_record(fcb, 0, write));
    std::vector<std::uint8_t> read;
    assert(fs.read_record(fcb, 0, read));
    assert(read == write);

    const std::size_t free_after_first_record = fs.free_block_count();
    assert(free_after_first_record < 241);

    std::vector<std::uint8_t> last_record(128, 0x7F);
    assert(fs.write_record(fcb, 127, last_record));
    read.clear();
    assert(fs.read_record(fcb, 127, read));
    assert(read == last_record);

    // Record 128 starts the next 16 KiB logical extent. The previous
    // implementation incorrectly called create(), which rejected the
    // existing filename and made this operation fail.
    std::vector<std::uint8_t> second_extent(128, 0x42);
    assert(fs.write_record(fcb, 128, second_extent));
    read.clear();
    assert(fs.read_record(fcb, 128, read));
    assert(read == second_extent);

    std::vector<cpm22::cpm::Fcb> results;
    const auto wildcard = cpm22::cpm::Fcb::from_string("HELLO.*", 0);
    assert(fs.search(wildcard, results));
    assert(results.size() == 2);
    assert(results.front().filename() == "HELLO.TXT");

    assert(fs.rename(fcb, cpm22::cpm::Fcb::from_string("WORLD.TXT", 0)));
    assert(!fs.open(fcb));
    const auto renamed = cpm22::cpm::Fcb::from_string("WORLD.TXT", 0);
    assert(fs.open(renamed));
    assert(fs.remove(renamed));
    assert(!fs.open(renamed));
    return 0;
}
