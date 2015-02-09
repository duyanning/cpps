#ifndef SHEBANGMAGIC_H
#define SHEBANGMAGIC_H

class ShebangMagic {
public:
    ShebangMagic(string script_name);
    ~ShebangMagic();
private:
    string m_script_name;
    bool m_changed = false;
};


#endif // SHEBANGMAGIC_H
