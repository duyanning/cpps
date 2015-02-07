// #!/home/duyanning/work/cpps/debug/cpps
#include <iostream>
#include <vector>
//#include <regex>
#include <string>
void foo();
// using  foo.cpp
// using bar.cpp
using namespace std;

int main(void)
{
    //cout << "hi" << endl;
    vector<int> v = {2, 4, 6, 8};
    for (auto x : v)
        cout << x << endl;

    // string s{"abc"};
    // regex pat{R"(ab)"};
    // smatch matches;
    // //regex_match(string("abc"), matches, pat);
    // if (regex_match(s, matches, pat)) {
    //     cout << "match ok\n";

    // }
    // cout << matches.size() << endl;

    foo();
    return 0;
}
