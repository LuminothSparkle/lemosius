#include <iostream>
#include <cassert>
#include <re2/re2.h>

using namespace std;
using namespace re2;

int main(int argc, char **argv)
{
    assert(RE2::FullMatch("hello", "h.*o"));
    cout << "Hello world!" << endl;
    return 0;
}

