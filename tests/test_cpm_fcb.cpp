#include "cpm/fcb.hpp"
#include <cassert>
int main(){
    assert(cpm22::cpm::Fcb::is_valid_filename("README.TXT"));
    assert(cpm22::cpm::Fcb::is_valid_filename("NOTES"));
    assert(!cpm22::cpm::Fcb::is_valid_filename("TOO-LONG-NAME.TXT"));
    assert(!cpm22::cpm::Fcb::is_valid_filename("README.LONG"));
    assert(!cpm22::cpm::Fcb::is_valid_filename("BAD/NAME.TXT"));
    const auto f=cpm22::cpm::Fcb::from_string("readme.txt",1);assert(f.user==1);assert(f.filename()=="README.TXT");return 0;}