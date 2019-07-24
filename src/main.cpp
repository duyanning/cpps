#include "config.h"
#include "global.h"
#include "Loggers.h"

int main(int argc, char* argv[])
try {
    boost::timer::cpu_timer timer;

    // 解析命令行选项，读取配置文件
    parse(argc, argv);

    // 扫描源文件，搜集信息
    collect();
    if (collect_only) return 0;

    // 构建
    bool success = build();
    if (!success) return 0;
    if (build_only) return 0;

    MINILOG(build_exe_timer_logger, "total " << timer.format(boost::timer::default_places, "%ws"));

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
