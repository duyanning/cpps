// this is the c++ version of check.py
// you can run it by "cpps check.cpp"
#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
    int retcode;

    string compile_cmd = "g++ -std=c++11 show-args.cpp -o show-args";
    system(compile_cmd.c_str());

    string run_cmd = "./show-args a b 'c d'> a.txt";
    system(run_cmd.c_str());

    string interpret_cmd = "cpps show-args.cpp a b 'c d' > b.txt";
    retcode = system(interpret_cmd.c_str());

    string compare_cmd = "diff a.txt b.txt";
    retcode = system(compare_cmd.c_str());

    if (retcode != 0)
        cout << "error found!" << endl;


}
