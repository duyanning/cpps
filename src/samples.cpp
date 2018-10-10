#include "samples.h"

char main_sample[] =
    R"(#include "config.h" // precompile config.h

using namespace std;

int main()
{
    cout << "welcome to cpps" << endl;
    return 0;
}
)";



char std_header_sample[] =
    R"(#ifndef STD_H
#define STD_H

#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <regex>

#endif // STD_H
)";


char class_h_sample[] =
    R"(#ifndef XXX_H
#define XXX_H

class XXX {
public:
    XXX();
    virtual ~XXX();
protected:
private:
};

#endif // XXX_H
)";


char class_cpp_sample[] =
    R"(#include "config.h"
#include "XXX.h"

XXX::XXX()
{
}

XXX::~XXX()
{
}

)";

