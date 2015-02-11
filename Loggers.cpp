#include "std.h"
#include "Loggers.h"

MiniLogger gch_magic_logger(cout, false);
MiniLogger build_exe_logger(cout, false);
MiniLogger build_exe_summay_logger(cout, true, &build_exe_logger);
MiniLogger build_exe_detail_logger(cout, false, &build_exe_logger);
MiniLogger build_gch_logger(cout, false);
MiniLogger build_gch_summay_logger(cout, true, &build_gch_logger);
MiniLogger build_gch_detail_logger(cout, false, &build_gch_logger);
