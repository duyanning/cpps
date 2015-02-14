#include "std.h"
#include "helpers.h"

fs::path get_home()
{
    return getpwuid(getuid())->pw_dir;
}

void move(const fs::path& old_p, const fs::path& new_p)
{
    boost::system::error_code ec;
    rename(old_p, new_p, ec);

    // rename失败，说明原来的位置跟目标位置不在同一个设备上，只好用copy&remove模拟
    if (ec) {
        // 复制会改变文件的时间戳，所以要调整
        time_t old_time = last_write_time(old_p);
        copy(old_p, new_p);
        last_write_time(new_p, old_time);

        remove(old_p);
    }
}

// 获得路径p的影子路径（即其在cache中的对应路径），p为绝对路径
fs::path shadow(fs::path p)
{
    //cout << "orig:" << p << endl;
    // 获取cache的位置
    fs::path cache_dir_name(".cpps/cache");
    fs::path cache_dir = get_home();
    cache_dir /= cache_dir_name;

    // 拼装
    fs::path right_part = p.relative_path();
    fs::path shadow_path = cache_dir;
    shadow_path /= right_part;

    //cout << "made:" << shadow_path << endl;

    return shadow_path;
}

bool can_write_in(fs::path dir)
{
    //fs::file_status s = status(dir);
    
    //cout << "perm: " << s.permissions() << " " << dir << endl;

    fs::path test_file = dir;
    test_file /= "cpps-test-permission";
    ofstream ofs(test_file.string());
    bool result = true;
    if (!ofs) {
        result = false;
    }
    else {
        ofs.close();
        remove(test_file);
        result = true;
    }

    return result;
}

bool is_a_cpp_src(fs::path f)
{
    fs::path suffix = f.extension();
    if (suffix == ".C"
        || suffix == ".cc"
        || suffix == ".cpp"
        || suffix == ".CPP"
        || suffix == ".c++"
        || suffix == ".cp"
        || suffix == ".cxx")
        return true;
    return false;
}

bool is_a_c_src(fs::path f)
{
    fs::path suffix = f.extension();
    if (suffix == ".c")
        return true;
    return false;
    
}
