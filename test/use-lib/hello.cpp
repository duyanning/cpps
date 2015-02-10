#include <iostream>
#include <string>
#include <boost/filesystem.hpp> // linklib boost_filesystem

using namespace std;
namespace fs = boost::filesystem;

int main()
{
    fs::path p("hello.cpp");
    if (exists(p))
        cout << p << " exists." << endl;

    return 0;
}
