#include "cpm/fcb.hpp"
#include <cassert>
int main(){const auto f=cpm22::cpm::Fcb::from_string("readme.txt",1);assert(f.user==1);assert(f.filename()=="README.TXT");return 0;}