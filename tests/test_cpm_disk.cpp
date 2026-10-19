#include "cpm/disk_image.hpp"
#include <cassert>
int main(){cpm22::cpm::DiskImage d;std::vector<std::uint8_t> w(128,0xE5),r;assert(d.write_sector(2,3,w));assert(d.read_sector(2,3,r));assert(r==w);return 0;}