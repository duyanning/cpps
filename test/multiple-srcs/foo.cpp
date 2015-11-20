#include <iostream>
#include "foo.h"
#include "bar.h" // using  bar.cpp

using namespace std;

void foo()
{
    cout << "this is foo" << endl;
    cout << "return value of bar is " << bar() << endl;
}
