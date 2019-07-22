#include <iostream>
#include "foo.h"
#include "bar.h" // using  bar.cpp

using namespace std;

void foo()
{
	bar();
}
