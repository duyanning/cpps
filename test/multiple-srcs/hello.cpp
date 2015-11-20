#include <iostream>
#include <vector>
#include <string>

#include "foo.h" // using  foo.cpp
#include "bar.h" // using bar.cpp

using namespace std;

int main()
{
    vector<int> v = {2, 4, 6, 8};
    for (auto x : v)
        cout << x << endl;

    foo();

    return 0;
}
