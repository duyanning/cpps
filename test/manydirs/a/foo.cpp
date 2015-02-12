#include <iostream>
#include "foo.h"
#include "../b/bar.h" // using  ../b/bar.cpp    precompile ../b/bar.h

using namespace std;

void foo()
{
    cout << "this is foo" << endl;
    cout << "return value of bar is " << bar() << endl;
}
