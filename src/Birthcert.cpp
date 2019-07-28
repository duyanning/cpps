#include "config.h"
#include "helpers.h"
#include "FileEntity.h"
#include "Loggers.h"

#include "Birthcert.h"

void Birthcert::addSig(string path, const FileSig& sig)
{
    m_map[path] = sig;
}

bool Birthcert::verifySig(string path, const FileSig& sig)
{
    //cout << "verifySig: " << path << endl;
    auto it = m_map.find(path);
    if (it == m_map.end()) {
        //cout << "not found in map" << endl;
        return true;
    }
        

    
    FileSig old_sig = it->second;

    if (old_sig == sig) {
        //cout << "equal" << endl;
        return true;
    }
        
    //cout << "not equal" << endl;
    return false;

}
