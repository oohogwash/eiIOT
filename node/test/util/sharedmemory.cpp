#include "sharedmemory.h"
#include "filecfg.h"
#include <iostream>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "log.h"
using namespace std;



namespace ei_share
{
SharedMemory::SharedMemory() : _open(false)
{

}

SharedMemory::~SharedMemory()
{

}

int SharedMemory::Create(string fileName)
{
    FileConfigHdr1 hdr1;


    if(access(fileName.c_str(), F_OK) != -1 )
    {
        return EEXIST;
    }
    else
    {
        PageAccess pa;
        PageAccessMgr mgr(pa);

        mgr.LoadToken("HAPP Manager", "ei2");

        PageConfigGroup pcg;
        fileCfg.pHdr = new FileConfigHdr1();

       // PageConfigGroupMgr pcgm(pcg);//pcgm(fileCfg.pHdr->pageConfigGroups[0]);
      //  pcgm.Init(PageConfigGroup::PageType::data, "Shm", "Shared memory Segment", 100240, 1024, pa);

        FileConfigHdr1 hdr;

        FileConfigHdrMgr fchm(hdr);

        PageConfigGroup pageConfigGroups[MAX_PAGE_GROUPS];
        PageConfigGroupMgr pcgm(pageConfigGroups[0]);
        pcgm.Init(PageConfigGroup::PageType::data, "shmdata", "Data shared mem segment", 100240, 1024, pa);



        PageConfig pageConfigs[MAX_PAGES];
        PageConfigMgr pcm(pageConfigs[0]);
        pcm.init("shm1", "First shared mem segment", pageConfigGroups[0].id);




      //  ei::core::diag::eiDebug("size of PageConfigMgr %ld, FileCfg %ld", sizeof(PageConfigMgr), sizeof(FileCfg));

        fchm.init(FileConfigHdr1::FileConfigType::sharedmem, pageConfigGroups, pageConfigs, 1, 1);

        std::cout << fchm.GetRecords("HAPP Manager").str();

     //   fileCfg.Create(fileName, hdr);

       // fileCfg._open = true;
    }
    return 0; //OK
}


void SharedMemory::Open(string fileName)
{
    fileCfg.pHdr->type = FileConfigHdr1::FileConfigType::sharedmem;
    strcpy(fileCfg.pHdr->pageConfigGroups[0].name, "Sharedmem");
    strcpy(fileCfg.pHdr->pageConfigGroups[0].name, "Shared memory interface");
    fileCfg.pHdr->pageConfigGroups[0].len = 102400;
    // fileCfg.pHdr->pageConfigGroups[0].access.
    PageAccessMgr mgr(fileCfg.pHdr->pageConfigGroups[0].access);

    mgr.LoadToken("HAPP Manager", "cmp");

    strcpy(fileCfg.pHdr->pageConfigs[0].name, "Sharedmem");
    strcpy(fileCfg.pHdr->pageConfigs[0].name, "Shared memory interface");
    //     strcpy fileCfg.pHdr->pageConfigs[0].uid


    //   fileCfg.pHdr->pageConfigGroups[]
    fileCfg.Open(fileName);
}

void SharedMemory::Close()
{
    //file
}

}


