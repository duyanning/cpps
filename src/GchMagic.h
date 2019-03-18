#ifndef GCHMAGIC_H
#define GCHMAGIC_H

class GchMagic {
public:
    GchMagic(vector<fs::path>& headers_to_precompile);
    ~GchMagic();
private:
    vector<fs::path>& m_headers_to_precompile;
};


#endif // GCHMAGIC_H
