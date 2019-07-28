#ifndef SAFECD_H
#define SAFECD_H

// 改变当前目录，并在析构时，恢复之前的当前目录
class SafeCD {
public:
    SafeCD(fs::path new_dir)
    {
        change(new_dir);
    }

    ~SafeCD()
    {
        restore();
    }

    void change(fs::path new_dir)
    {
        m_old_dir = fs::current_path();
        current_path(new_dir);
        m_is_restored = false;
    }
    
    void restore()
    {
        if (m_is_restored)
            return;

        fs::current_path(m_old_dir);
    }
private:
    fs::path m_old_dir;
    bool m_is_restored = false;
};



#endif // SAFECD_H
