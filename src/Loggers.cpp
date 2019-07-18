#include "config.h"
#include "Loggers.h"

MiniLogger gch_magic_logger(cout, false);
MiniLogger build_exe_logger(cout, true);
MiniLogger build_exe_summay_logger(cout, false, &build_exe_logger);
MiniLogger build_exe_detail_logger(cout, false, &build_exe_logger);
MiniLogger build_gch_logger(cout, true);
MiniLogger build_gch_summay_logger(cout, false, &build_gch_logger);
MiniLogger build_gch_detail_logger(cout, false, &build_gch_logger);
MiniLogger dep_graph_logger(cout, true);
MiniLogger collect_info_logger(cout, false);
MiniLogger shebang_logger(cout, false);
MiniLogger perm_logger(cout, false); // 与文件读写权限有关
MiniLogger birthcert_logger(cout, false);
MiniLogger update_logger(cout, false);
//MiniLogger action_logger(cout, true);
