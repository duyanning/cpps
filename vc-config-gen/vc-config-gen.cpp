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
//			auto semicolon_pos = line.find(";");
//			cout << R"(compiler-dir = ")"
//				<< line.substr(5, semicolon_pos - 5)
//				<< R"(")" << endl;
//			cout << "\n\n\n";
            // 有时这个目录在path的第一截，有时不在第一截
			auto msvc_pos = line.find("VC\\Tools\\MSVC");
			auto previous_semicolon_pos = line.rfind(";", msvc_pos);
			if (previous_semicolon_pos == string::npos) {
                previous_semicolon_pos = line.rfind("=", msvc_pos);
			}
			auto next_semicolon_pos = line.find(";", msvc_pos);
			cout << R"(compiler-dir = ")"
				<< line.substr(previous_semicolon_pos + 1, next_semicolon_pos - previous_semicolon_pos - 1)
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
