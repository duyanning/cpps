// 该文件仅供开发时使用，对于从源码安装的用户，需要从config.h.in来生成该文件
#ifndef CONFIG_H
#define CONFIG_H


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

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/timer/timer.hpp>

#define HAVE_REGEX

#ifdef HAVE_REGEX
#include <regex>
#else
#include <boost/regex.hpp>
#endif


#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

#ifdef _WIN32
#include <process.h>
#endif

#include "MiniLogger.h"

using namespace std;
namespace fs = boost::filesystem;
namespace po = boost::program_options;
namespace al = boost::algorithm;


#endif // CONFIG_H
