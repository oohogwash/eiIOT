#include "FileCfg.h"
#include <iostream>
#include"guid.h"

int PageAccessMgr::hash(const char *str)
{
    const static int N_BUCKETS = 100;
    int h = 0;
    while (*str)
       h = h << 1 ^ *str++;
    return (h > 0 ? h : -h) % N_BUCKETS;
}

void PageAccessMgr::LoadToken( const char * token, const char * algorithm)
{
    char buffer[37];
    strncpy(pageAccess.algorithm, algorithm, ALGID_LEN);
    pageAccess.algorithm[ALGID_LEN] = NULL;
    strncpy(pageAccess.token, token, UIDLEN);
    pageAccess.token[UIDLEN] = NULL;
    pageAccess.algorithmId = hash(algorithm);
    if(pageAccess.algorithmId == paa_cmp)
    {
        strncpy(pageAccess.rToken, GuidGenerator::newGuid().getBuffer(buffer),UIDLEN);
        pageAccess.rToken[UIDLEN] = NULL;
        strncpy(pageAccess.wToken, GuidGenerator::newGuid().getBuffer(buffer), UIDLEN);
        pageAccess.wToken[UIDLEN] = NULL;
    }
    else
    {
        // these are not used, just to show its using an internal id
        strncpy(pageAccess.rToken, "ei",UIDLEN);
        strncpy(pageAccess.wToken, "ei", UIDLEN);
    }
}

PageAccessMode PageAccessMgr::Validate(const char clientToken)
{
    switch(pageAccess.algorithmId)
    {
    case 94: //cmp
        return pam_read;
    case 75: //ei1
        return pam_write;
    case 72: //ei2
        return pam_read;
    default:
        return pam_none;
    }
}

FileCfg::FileCfg()
{

}

std::stringstream PageConfigGroupMgr::GetRecord()
{
    std::stringstream buff;
    buff<< "gid:"<<pcg.id<<",name:"<<pcg.name<<",desc:"<<pcg.desc<<",type:"<<pcg.type<<",rToken:"<<pcg.access.rToken<<",wToken:"<<pcg.access.wToken<<std::endl;
    return buff;
}


void PageConfigGroupMgr::Init( const PageConfigGroup::PageType pageType, const char * name, const char * desc, long len, long blocksize, PageAccess & pageAccess)
{
//      pcg.type = pageType;
    char buffer[UIDLEN+1];
    strncpy(pcg.id, GuidGenerator::newGuid().getBuffer(buffer),UIDLEN);
    pcg.id[UIDLEN] = NULL;
    std::strncpy(pcg.name, name, MAX_PAGE_NAME_SIZE -1);
    std::strncpy(pcg.desc, desc, MAX_PAGE_DESC_SIZE-1);
    pcg.len = len;
    pcg.blockSize = blocksize;
    ::memcpy(&pcg.access, &pageAccess, sizeof(PageAccess));
}

void PageConfigMgr::init( const char * name, const char * desc, const char * gid)
{
    char buffer[UIDLEN+1];
    strncpy(pc.id, GuidGenerator::newGuid().getBuffer(buffer), UIDLEN);
    pc.id[UIDLEN] = NULL;
    strcpy(pc.name, name);
    strcpy(pc.desc, desc);
    strncpy(pc.gid, gid, UIDLEN);
    pc.gid[UIDLEN] = NULL;
}

FileConfigHdrMgr::FileConfigHdrMgr(FileConfigHdr1 & fileConfigHdr) :hdr(fileConfigHdr)
{
    hdr =fileConfigHdr;
}
FileCfg::~FileCfg()
{

}

void FileCfg::Create( std::string filename, FileConfigHdr1 & hdr)
{

    mapio.Open(filename, pageSize);

    long * phdrSize = (long *) mapio.Map(0, pageSize);

    *phdrSize = hdr.sz;
    phdrSize++;
    *phdrSize = 1; // header version

    pHdr = (FileConfigHdr1 *)mapio.Map(2*sizeof(long), hdr.sz);
    std::cout << " copy memory" << hdr.sz << " " << sizeof(hdr) << std::endl;
    memcpy(pHdr, &hdr, sizeof(hdr));
}

void FileCfg::Close()
{
     mapio.Close();
}


void FileCfg::Open( std::string filename)
{
    mapio.Open(filename, pageSize);
    FileConfigHdr1  hdr;
    pHdr = (FileConfigHdr1 *)mapio.Map(0, hdr.sz);

}

//PageGroupAccessRecord PageConfigGroupMgr::GetPageGroupAccessRecord()
//{
//     PageGroupAccessRecord par;
//     par.id = pcg.id;
//     par.name = pcg.name;
//     par.rToken = pcg.access.rToken;
//     par.wToken = pcg.access.wToken;
//}

//PageRecord PageConfigMgr::GetPageRecord()
//{
//     PageGroupAccessRecord par;
//     par.id = pcg.id;
//     par.name = pcg.name;
//     par.rToken = pcg.access.rToken;
//     par.wToken = pcg.access.wToken;
//}

std::stringstream FileConfigHdrMgr::GetRecords(char * token)
{
    std::stringstream records;
        for(int idx =0; idx < hdr.numGroups; idx++)
        {
            if(!strcmp(token, hdr.pageConfigGroups[idx].access.token) )
            {
               PageConfigGroupMgr pcg(hdr.pageConfigGroups[idx]);
               records << pcg.GetRecord().str();

               for(int pidx =0; pidx < hdr.numPages; pidx++)
               {
                   if(!strcmp(hdr.pageConfigGroups[idx].id, hdr.pageConfigs[pidx].gid) )
                   {
                      PageConfigMgr pc(hdr.pageConfigs[pidx]);
                      records << pc.GetRecord().str();
                   }



            }
        }

        }
       return records;
}

//std::list<PageAccessRecord> FileConfigHdrMgr::GetPageAccessRecords(std::string token)
//{
//    std::list<PageAccessRecord> records;
//    for(int idx =0; idx < hdr.numGroups; idx++)
//    {
//        if(token == hdr.pageConfigGroups[idx].access.token )
//        {
//            PageConfigGroupMgr pcg(hdr.pageConfigGroups[idx]);
//            records.push_back(pcg.GetPageAccessRecord());
//        }
//    }
//    records;
//}


void FileCfg::InitFile(std::string file, long pagesize, long maxNumMaps, long maxNumLocks, long maxNumSnapshots)
{







    ei::MapIO mapio;
    mapio.Open(file, pagesize);


    std::cout << "file opened";
    long len = INDEXOFFSET + maxNumLocks * sizeof(LockRecord) +
            maxNumMaps * sizeof(MapRecord) + maxNumSnapshots * sizeof(SnapshotRecord);

    char * ptr = mapio.Map(0, len);

   std::cout << "mem mapped";

    memset(ptr, 0, len);
    *ptr = pagesize;
    ptr+= sizeof(long);
    *ptr = maxNumLocks;

    ptr+= sizeof(long);
    *ptr = maxNumMaps;

    ptr+= sizeof(long);
    *ptr = maxNumSnapshots;

    mapio.Close();

    std::cout << "closed file";

   // SnapshotRecord snapshots [MAX_SNAPSHOTS];

   // LockRecord locks [MAX_LOCKS];


   // MapRecord maps[maxNumMaps];

}
