#include "config.h"
#include "global.h"
#include "Loggers.h"

int main(int argc, char* argv[])
try {

    // 解析命令行选项，读取配置文件
    parse(argc, argv);

    boost::timer::cpu_timer stage_timer, total_timer;

    // 扫描源文件，搜集信息
    stage_timer.start();
    collect();
    MINILOG(build_exe_timer_logger, stage_timer.format(boost::timer::default_places, "%ws") << " COLLECT");
    if (collect_only) return 0;

    // 构建
    stage_timer.start();
    bool success = build();
    MINILOG(build_exe_timer_logger, stage_timer.format(boost::timer::default_places, "%ws") << " BUILD");
    if (!success) return 0;
    if (build_only) return 0;

    MINILOG(build_exe_timer_logger, total_timer.format(boost::timer::default_places, "%ws") << " TOTAL = COLLECT + BUILD");

    // 运行
    run();

    return 0;
}
catch (const po::error& ex) {
    std::cerr << ex.what() << '\n';
    return 1;
}
catch (int exit_code) {
    return exit_code;
}
