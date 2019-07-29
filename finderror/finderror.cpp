#include <iostream>
#include <string>

using namespace std;

// vc的报错信息格式可参考
// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-errors-1/c-cpp-build-errors?view=vs-2019
int main()
{
	int exit_code = 0;
	while (cin) {
		string line;
		getline(cin, line);
		// ): error
		if (cin) { // 避免在最后一行多加一个\n
			cout << line << endl;
		}
		else {
			cout << line;
		}
		if (line.find("): error ") != std::string::npos) {
			exit_code = 1;
		}
        else if (line.find("): fatal error ") != std::string::npos) {
            exit_code = 1;
        }
	}

	return exit_code;
}
