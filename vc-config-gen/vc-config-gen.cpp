#include "pch.h"

using namespace std;

void get_parts(vector<string>& parts, const string& s)
{
	std::string delimiters(";"); // 支持多个分界符
	boost::split(parts, s, boost::is_any_of(delimiters));
}

void gen_config_string(const string& prefix, const vector<string>& parts)
{
	for (auto p : parts) {
		if (p != "") {
			cout << prefix << R"( = ")" << p << R"(")" << endl;
		}
		
	}
}

int main()
{
	system("SET > vars.txt");
	ifstream in("vars.txt");
	string line;
	while (getline(in, line)) {

		//if (line.find("Path=") == 0) {
		if (boost::ifind_first(line, "PATH=").begin() == line.begin()) {

			//cout << line << endl;
			auto semicolon_pos = line.find(";");
			cout << R"(compiler-dir = ")"
				<< line.substr(5, semicolon_pos - 5)
				<< R"(")" << endl;
			cout << "\n\n\n";
		}

		if (line.find("INCLUDE=") != string::npos) {

			//cout << line << endl;
			vector<string> parts;
			get_parts(parts, line.substr(8));
			gen_config_string("include-dir", parts);
			cout << "\n\n\n";
		}

		if (line.find("LIB=") != string::npos) {

			//cout << line << endl;
			vector<string> parts;
			get_parts(parts, line.substr(4));
			gen_config_string("lib-dir", parts);
			cout << "\n\n\n";
		}


	}

	in.close();
	remove("vars.txt");

	return 0;
}
