#include "config.h"
#include "global.h"
#include "samples.h"


void generate_main_file(string main_file_name)
{
	ofstream f;

	if (fs::exists(main_file_name)) {
		cout << main_file_name << " already exists." << endl;
		return;
	}
	f.open(main_file_name);
	f << main_sample;
	f.close();


	if (fs::exists("std.h")) {
		cout << "std.h already exists." << endl;
		return;
	}
	f.open("std.h");
	f << std_header_sample;
	f.close();
}

void generate_class_files(string class_name)
{
	string h_name = class_name + ".h";
	string cpp_name = class_name + ".cpp";

	ofstream f;
	regex pat;
	string format;

	if (fs::exists(h_name)) {
		cout << h_name << " already exists." << endl;
		return;
	}
	f.open(h_name);
	pat = "XXX_H";
	format = al::to_upper_copy(class_name) + "_H";
	string h_content = regex_replace(string(class_h_sample), pat, format);

	pat = "XXX";
	format = class_name;
	h_content = regex_replace(h_content, pat, format);
	f << h_content;
	f.close();


	if (fs::exists(cpp_name)) {
		cout << cpp_name << " already exists." << endl;
		return;
	}
	f.open(cpp_name);
	pat = "XXX";
	format = class_name;
	string cpp_content = regex_replace(string(class_cpp_sample), pat, format);
	f << cpp_content;
	f.close();
}
