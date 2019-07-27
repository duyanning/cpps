#include "config.h"
#include "VcObj2ExeAction.h"
#include "FileEntity.h"
#include "Loggers.h"
#include "global.h"

VcObj2ExeAction::VcObj2ExeAction()
{
}

VcObj2ExeActionPtr makeVcObj2ExeAction()
{
    return VcObj2ExeActionPtr(new VcObj2ExeAction);
}

bool VcObj2ExeAction::execute(const DepInfo& info)
{
    // 如果exe依赖的某个.o更新失败，那就无法生成exe
    if (!info.failed.empty()) {
        //cout << "VcObj2ExeAction failed" << endl;
        return false;
    }

    boost::timer::cpu_timer timer;
    
    // 构造命令行
    FileEntityPtr exe = static_pointer_cast<FileEntity>(info.target);
    fs::path exe_path = exe->path();

    fs::path dep_path = exe_path;
    dep_path += ".d";

    // fs::path birthcert_path = exe_path;
    // birthcert_path += ".birthcert";

	string cmd = R"(")"; // cmd的怪癖，传给system函数的东西，前后还得一个引号。 参考： https://stackoverflow.com/questions/9964865/c-system-not-working-when-there-are-spaces-in-two-different-parameters

    cmd += link_cmd;

	cmd += " /Fe:";

    cmd += " ";

    cmd += exe_path.string();

    for (auto p : info.all) {
        FileEntityPtr f = static_pointer_cast<FileEntity>(p);
        cmd = cmd + " " + f->path().string();
    }

    // 下面这些东西必须放在文件名之后
    cmd += " ";
    cmd += link_cmd_libs;

    // 以及配置文件中lib指定的库
    if (vm.count("vc.linklib")) {
        cmd += " ";
        for (auto line : vm["vc.linklib"].as<vector<string>>()) {
            cmd += line;
            cmd += " ";
        }
    }

    cmd += " /link";
    cmd += " ";
    cmd += link_cmd_lib_dirs;

    // -l 选项的位置很重要，必须放在使用它的.o文件之后
    cmd += " ";
    cmd += compiler_specific_extra_link_flags;


	cmd += R"(")"; // 传给system函数的东西最后额外的引号

    MINILOG(build_exe_summay_logger, "linking " << exe_path.filename().string());
    MINILOG(build_exe_detail_logger, cmd);

    // 确保目录存在
    create_directories(exe_path.parent_path());

    // 链接
    int gcc_status;
    gcc_status = system(cmd.c_str());
    if (gcc_status)
        return false;

    exe->generate_birth_cert();

    MINILOG(build_exe_timer_logger, timer.format(boost::timer::default_places, "%ws") << " linking " << exe_path.filename());

    return true;
}
