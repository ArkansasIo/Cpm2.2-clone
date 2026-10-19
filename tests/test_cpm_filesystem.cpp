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
    assert(fs.free_block_count() < 241);

    std::vector<cpm22::cpm::Fcb> results;
    const auto wildcard = cpm22::cpm::Fcb::from_string("HELLO.*", 0);
    assert(fs.search(wildcard, results));
    assert(results.size() == 1);
    assert(results.front().filename() == "HELLO.TXT");

    assert(fs.rename(fcb, cpm22::cpm::Fcb::from_string("WORLD.TXT", 0)));
    assert(!fs.open(fcb));
    const auto renamed = cpm22::cpm::Fcb::from_string("WORLD.TXT", 0);
    assert(fs.open(renamed));
    assert(fs.remove(renamed));
    assert(!fs.open(renamed));
    return 0;
}
