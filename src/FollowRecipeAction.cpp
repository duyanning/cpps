#include "config.h"
#include "global.h"
#include "FollowRecipeAction.h"
#include "FileEntity.h"
#include "Loggers.h"
#include "helpers.h"


bool FollowRecipeAction::execute(const DepInfo& info)
{
    //cout << "get here" << endl;
    // 因为一个规则可能生成多个target，所以本action会挂在多个节点上。执行一次就行了。
    if (m_executed) return true;
    m_executed = true;

    // // 一个.o依赖于一个.cpp跟若干个.h
    // // .h更新失败(比如.h不存在)无所谓，因为有可能目前已经不依赖该.h
    // // 但.cpp不存在将无法产生.o
    // if (!info.failed.empty()) {
    //     for (auto pre : info.failed) {
    //         FileEntityPtr fe = static_pointer_cast<FileEntity>(pre);
    //         if (is_a_cpp_src(fe->path())) return false;
    //     }
    // }

    boost::timer::cpu_timer timer;

    _MINILOG(build_exe_summay_logger, "making ");
    MINILOGBLK_IF(
        build_exe_summay_logger.is_enabled(), build_exe_summay_logger,
        for (auto t : m_rule.targets) {
            os << t.filename().string();
            os << " ";
        }
        os << endl;
    );


    // 确保目录存在
    for (auto t : m_rule.targets) {
        create_directories(t.parent_path());
    }

    for (auto c : m_rule.commands) {
        //cout << "executing command: " << c << endl;
        MINILOG(build_exe_detail_logger, c);
        system(c.c_str());
    }

    //// 构造命令行
    //FileEntityPtr cpp = static_pointer_cast<FileEntity>(info.all[0]);
    //fs::path cpp_path = cpp->path();

    ////VulnerableFileEntityPtr obj = static_pointer_cast<VulnerableFileEntity>(info.target);
    //FileEntityPtr obj = static_pointer_cast<FileEntity>(info.target);
    //fs::path obj_path = obj->path();

    //fs::path dep_path = obj_path;
    //dep_path += ".d";

    //// fs::path birthcert_path = obj_path;
    //// birthcert_path += ".birthcert";

    //string cmd = compile_cpp_cmd;

    //cmd += compiler_specific_extra_compile_flags[cpp_path.string()];
    //
    //cmd += " ";
    //cmd += compile_cmd_include_dirs;

    //cmd += " -o";

    //cmd += " ";
    //cmd += obj_path.string();

    //cmd += " ";
    //cmd += cpp_path.string();

    ////cmd += " -fpch-deps -MMD -MF " + dep_path.string();
    ////cmd += " -MMD -MF " + dep_path.string(); // todo: clang不支持-fpch-deps
    //cmd += " " + cc_info[cc].option_dep + " " + dep_path.string();

    //MINILOG(build_exe_summay_logger, "compiling " << cpp_path.filename().string());
    //MINILOG(build_exe_detail_logger, cmd);

    //// 确保目录存在
    //create_directories(obj_path.parent_path());

    //// 产生.o文件和.d文件
    //int gcc_status;
    //gcc_status = system(cmd.c_str());
    //// 函数system给出的就是gcc的返回值
    //// 在shell下执行gcc后，可用echo $?来查看该返回值
    //// 有编译错误时，该返回值为1，无错误时为0
    //// 即便有警告，仍旧是0。
    ////cout << "cpp2obj:" << gcc_status << endl;
    //if (gcc_status)             // 非0就是代表失败了
    //    return false;

    // 为每个target产生出生证明文件
    for (auto t : m_rule.targets) {
        FileEntityPtr a_target = makeFileEntity(t);
        a_target->generate_birth_cert();
    }
    
    MINILOG(build_exe_timer_logger, timer.format(boost::timer::default_places, "%ws") << " making ");

    return true;
}
