#include "config.h"
#include "global.h"
#include "FollowRecipeAction.h"
#include "FileEntity.h"
#include "Loggers.h"
#include "helpers.h"
#include "SafeCD.h"


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

    SafeCD scd(m_rule.dir);

    // todo：如果要处理命令中的变量，应该在此处进行。
    // 我也不确定了：或许应该在处理target与prerequisites中的变量时一并处理
    // 此处或许可以处理$@、$?、$^、$+等自动变量(含义可参考《Managing Projects with GNU Make_3rd_中文版》26页)

    for (auto c : m_rule.commands) {
        //cout << "executing command: " << c << endl;
        MINILOG(build_exe_detail_logger, c);
        int status;
        status = system(c.c_str());
        if (status)             // 非0就是代表失败了
           return false;
    }
    
    scd.restore();

    // 为每个target产生出生证明文件
    for (auto t : m_rule.targets) {
        FileEntityPtr a_target = makeFileEntity(t);
        a_target->generate_birth_cert();
    }
    
    MINILOG(build_exe_timer_logger, timer.format(boost::timer::default_places, "%ws") << " making ");

    return true;
}
