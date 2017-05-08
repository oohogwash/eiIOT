#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H
#include "fileCfg.h"

namespace ei_share{
class SharedMemory
{
    bool _open;
    FileCfg fileCfg;
    FileConfigHdr1 hdr;
public:
    SharedMemory();
    ~SharedMemory();
    void Open(std::string fileName);
    int Create(std::string fileName);
    void Close();
};
}
#endif // SHAREDMEMORY_H
