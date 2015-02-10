#include "std.h"
#include "ShebangMagic.h"

ShebangMagic::ShebangMagic(string script_name)
    :
    m_script_name(script_name)
{
    fstream f(m_script_name, ios::in | ios::out);
    if (f.get() != '#')
        return;
    if (f.get() != '!')
        return;
    MINILOG0("shebang found");

    fs::path p(m_script_name);
    time_t old_time = last_write_time(p);

    f.seekp(0);
    f.write("//", 2);

    f.close();

    last_write_time(p, old_time);

    m_changed = true;
}

ShebangMagic::~ShebangMagic()
{
    if (!m_changed)
        return;


    fs::path p(m_script_name);
    time_t old_time = last_write_time(p);

    // 从//改回#!
    fstream f(m_script_name, ios::in | ios::out);
    f.seekp(0);
    f.write("#!", 2);
    f.close();
    MINILOG0("shebang changed back");

    last_write_time(p, old_time);
}

