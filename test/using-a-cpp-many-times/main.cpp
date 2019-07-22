#include <iostream>
#include <vector>
#include <string>

#include "foo.h" // using foo.cpp
#include "bar.h" // using bar.cpp

using namespace std;

int main()
{
    foo();
    bar();

    return 0;
}
