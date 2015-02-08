#include "std.h"
#include "MiniLogger.h"

using namespace std;

// 日志开启关闭，分三级控制：
// 1，即可整体开启关闭
// 2，也可按不同的logger开启关闭
// 3，下级logger是否开启或关闭，还受上级logger状态的控制
MiniLogger MiniLogger::default_logger(cout, true);
bool MiniLogger::disable_all_loggers = false;
