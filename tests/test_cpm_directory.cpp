#include "cpm/directory.hpp"
#include <cassert>

int main() {
    cpm22::cpm::Directory dir;
    assert(dir.add({0, "hello", "txt", 42, false}));
    assert(dir.contains("HELLO", "TXT"));
    assert(!dir.add({0, "hello", "txt", 10, false}));
    assert(dir.list().size() == 1);
    assert(dir.remove("HELLO", "TXT"));
    assert(!dir.contains("hello", "txt"));
    return 0;
}
