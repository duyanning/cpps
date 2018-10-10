#include <iostream>

#include "bar.h" // using qux.cpp
// '// using qux.cpp' following '#include "bar.h"' causes qux.cpp to be compiled and linked

#include "foo.h" // usingcpp
// '// usingcpp' following '#include "foo.h"' causes foo.cpp to be compiled and linked
// same as '// using foo.cpp'

int main()
{
    std::cout << add(1, 2) << std::endl;
    std::cout << ddiv(9, 2) << std::endl;
    return 0;
}
