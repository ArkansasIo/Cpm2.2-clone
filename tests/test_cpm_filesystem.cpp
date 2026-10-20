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

    // Copy the source name to a separate variable before renaming. The
    // filesystem API treats FCBs as fixed-width fields, not C strings.
    const auto original = cpm22::cpm::Fcb::from_string("HELLO.TXT", 0);
    const auto renamed = cpm22::cpm::Fcb::from_string("WORLD.TXT", 0);
    assert(fs.rename(original, renamed));
    assert(fs.open(renamed));
    assert(!fs.open(original));
    assert(fs.remove(renamed));
    assert(!fs.open(renamed));

    // Rename rejects illegal CP/M 8.3 replacement names.
    const auto plain = cpm22::cpm::Fcb::from_string("README.TXT", 0);
    assert(fs.create(plain));
    assert(!fs.rename(plain, cpm22::cpm::Fcb::from_string("TOO-LONG-NAME.TXT", 0)));
    assert(!fs.rename(plain, cpm22::cpm::Fcb::from_string("README.LONG", 0)));
    assert(fs.open(plain));
    assert(fs.rename(plain, cpm22::cpm::Fcb::from_string("NOTES", 0)));
    assert(!fs.open(plain));
    assert(fs.open(cpm22::cpm::Fcb::from_string("NOTES", 0)));
    return 0;
}
