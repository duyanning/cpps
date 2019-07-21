#include "config.h"
#include "helpers.h"

#ifdef _WIN32
#include <Shlobj.h>
#endif // _WIN32

fs::path get_home()
{
    #ifdef _WIN32
    //return "c:\\";
    char buf[MAX_PATH];
    SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, buf);
    return buf;
    #else
    return getpwuid(getuid())->pw_dir;
    #endif // _WIN32
}

void move(const fs::path& old_p, const fs::path& new_p)
{
    boost::system::error_code ec;
    rename(old_p, new_p, ec);

    // rename失败，说明原来的位置跟目标位置不在同一个设备上，只好用copy&remove模拟
    if (ec) {
        // 复制会改变文件的时间戳，所以要调整
        //cout << "aaaaaaaaaaaaaaaa\n";
        time_t old_time = last_write_time(old_p);
        copy(old_p, new_p);
        //cout << "bbbbbbbbbbbbbbb\n";
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

    // 如果p已经位于cache之中，直接返回
    if (p.string().find(cache_dir_name.string()) != std::string::npos) {
        return p;
    }


    fs::path cache_dir = get_home();
    cache_dir /= cache_dir_name;

    // 拼装
    fs::path right_part = p.relative_path();
    fs::path shadow_path = cache_dir;
    shadow_path /= right_part;

    //cout << "made:" << shadow_path << endl;

    return shadow_path.make_preferred();
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
        || suffix == ".cxx"
        || suffix == ".c")
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

void safe_remove(fs::path f)
{
    if (exists(f))
        remove(f);
}

// 可移植的putenv
void put_env(const char* envstring)
{
#if defined(_MSC_VER)
	_putenv(envstring);
#elif defined(__MINGW32__)
	assert(false);
#else
	putenv(const_cast<char*>(envstring));
#endif

}