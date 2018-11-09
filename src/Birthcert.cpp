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
    auto it = m_map.find(path);
    if (it == m_map.end())
        return true;

    
    FileSig old_sig = it->second;

    if (old_sig == sig)
        return true;

    return false;

}
