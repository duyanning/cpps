#include "std.h"
#include "helpers.h"

// 获得路径p的影子路径（即其在cache中的对应路径）
fs::path shadow(fs::path p)
{
    //cout << "orig:" << p << endl;
    // 获取cache的位置
    fs::path home_dir(getpwuid(getuid())->pw_dir);
    fs::path cache_dir_name(".cpps/cache");
    fs::path cache_dir = home_dir;
    cache_dir /= cache_dir_name;

    // 拼装
    fs::path right_part = p.relative_path();
    fs::path shadow_path = cache_dir;
    shadow_path /= right_part;

    //cout << "made:" << shadow_path << endl;

    return shadow_path;
}
