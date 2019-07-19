#include <iostream>
#include <boost/program_options.hpp>

// vc-linklib boost_program_options-vc140-mt
// mingw-linklib libboost_program_options-mgw63-mt-x32-1_66
// 其实使用linklib就可以，但为了可移植性，使用了<compiler>-linklib这种形式。
// 为啥这么麻烦？没办法，windows下，boost的库名字中都带着版本号

// vc-extra-compile-flags:  /source-charset:utf-8 
// 本文件是utf-8编码，为了让vc不发出警告，需要上面这个编译选项
  
using namespace boost::program_options;

void on_age(int age)
{
  std::cout << "On age: " << age << '\n';
}

int main(int argc, const char *argv[])
{
  try
  {
    options_description desc{"Options"};
    desc.add_options()
      ("help,h", "Help screen")
      ("pi", value<float>()->default_value(3.14f), "Pi")
      ("age", value<int>()->notifier(on_age), "Age");

    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);

    if (vm.count("help"))
      std::cout << desc << '\n';
    else if (vm.count("age"))
      std::cout << "Age: " << vm["age"].as<int>() << '\n';
    else if (vm.count("pi"))
      std::cout << "Pi: " << vm["pi"].as<float>() << '\n';
  }
  catch (const error &ex)
  {
    std::cerr << ex.what() << '\n';
  }
}