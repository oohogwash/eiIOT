#ifndef FILECFG_H
#define FILECFG_H

#include <string>
#include "mapio.h"
#include <memory.h>
#include <sstream>
#include <list>

const int MAX_SNAPSHOTS = 1024;
const int MAX_LOCKS = 1024;
const int MAX_MAPS = 1024;
const long PAGESIZE = 4096;
const long PAGESIZEOFFSET = 0;
const long INDEXOFFSET = 4 * sizeof(long);
const long SZ_MAP_INFO_REC = 128;
const long MAX_MAP_RECORDS = 128;
const int FILE_CONFIG_SIZE = 20400;
const int MAX_PAGE_NAME_SIZE =64;
const int MAX_PAGE_DESC_SIZE=128;
const int MAX_PAGES = 128;
const int MAX_PAGE_GROUPS = 10;
const int UIDLEN = 36;
const int ALGID_LEN = 10;


class PageGroupAccessRecord
{
public:
    std::string id;
    std::string name;
    std::string rToken;
    std::string wToken;

};

class PageRecord
{
public:
    std::string Id;
    std::string name;
    std::string gid;
};



struct Version
{
    int major;
    int minor;
    int batch;
    int build;
};

struct MapInfo
{
    enum MapInfoType
    {
        ReadOnlyMap = 0,
        ReadWriteMap,
        LockedReadWriteMap
    };

    char name[64];
    MapInfoType type = MapInfoType::ReadOnlyMap;
    long start;
    long len;
};

typedef struct {
    union
    {
        MapInfo rec[MAX_MAP_RECORDS];
        char buff[SZ_MAP_INFO_REC];
    };
}LockInfoRec;

typedef struct
{
    typedef struct
    {
        int id;
        bool writeAccess = false;
     }Access;

  //  Access uid[16]; move this to an access segment
  //  Access gid[16];
    char token[UIDLEN+1];
    char rToken[UIDLEN+1];
    char wToken[UIDLEN+1];
    char algorithm[ALGID_LEN+1];
    int algorithmId;
     long timeout= 0; // the time to hold on to tokens for in s, < 1 = unlimited
}PageAccess;

typedef struct
{
    typedef struct
    {
        char token[UIDLEN];
        long timeout;
    }TokenInfo;
    TokenInfo rTokens[150];
    TokenInfo wTokens[150];

}AccessTokens;


typedef enum
{
    pam_none,
    pam_read,
    pam_write
}PageAccessMode;

typedef enum
{
    paa_cmp = 94,

}PageAccessAlgorithm;


typedef struct
{
    char id[UIDLEN+1];
    typedef enum
    {
        undefined = 0,
        header,
        data,
        index,
        queue,
        config
    }PageType;
    long len;
    long blockSize;
    PageType type = PageType::undefined;
    char name[MAX_PAGE_NAME_SIZE];
    char desc [MAX_PAGE_DESC_SIZE];
    PageAccess access;
    AccessTokens accessTokens;
} PageConfigGroup;


class PageConfigGroupMgr
{
    PageConfigGroup &pcg;
public:
    PageConfigGroupMgr( PageConfigGroup & pageConfigGroup) :pcg(pageConfigGroup){};
    void Init( const PageConfigGroup::PageType pageType, const char * name, const char * desc, long len, long blocksize, PageAccess & pageAccess);
    std::stringstream GetRecord();

};


class PageAccessMgr
{
    PageAccess & pageAccess;
public:
    PageAccessMgr( PageAccess & pageAccessIn): pageAccess(pageAccessIn){}
    int hash(const char *str);
    void LoadToken( const char * token, const char * algorithm);
    PageAccessMode Validate(const char clientToken);
};





typedef struct {
    char name [MAX_PAGE_NAME_SIZE];
    char id[UIDLEN+1]; // guiid text
    char desc [MAX_PAGE_DESC_SIZE];
    long start;
    long size;
    char gid[UIDLEN+1]; // the group that configures this page

}PageConfig;

class PageConfigMgr
{
    PageConfig &pc;
public:
    PageConfigMgr(PageConfig & pageConfig) : pc(pageConfig){}
    void init( const char * name, const char * desc, const char * gid);
    std::stringstream GetRecord()
    {
        std::stringstream buff;

        buff << "pageid:" << pc.id << ",name:" << pc.name <<",desc:"<<pc.desc <<",start:" << pc.start <<",size:" << pc.size << ",gid:" <<pc.gid<<std::endl;
        return buff;
    }
};



typedef struct
{
    typedef enum
    {
        sharedmem = 0,
        indexedFile,
        dataFile,
        queue,
        circularBuffer,
        FIFO,
        stream,
        custom
    }FileConfigType;

    FileConfigType type =  FileConfigType::indexedFile ; // just a key to the structure and semantics
    long sz = FILE_CONFIG_SIZE;
    Version version;

    int numPages=0;
    int numGroups=0;
    PageConfigGroup pageConfigGroups[MAX_PAGE_GROUPS];
    PageConfig pageConfigs[MAX_PAGES];


}FileConfigHdr1;

class FileConfigHdrMgr
{
    FileConfigHdr1 hdrggg;
    FileConfigHdr1 &hdr;
public:
    FileConfigHdrMgr(FileConfigHdr1 & fileConfigHdr);
    void init( FileConfigHdr1::FileConfigType type, const PageConfigGroup (& pageConfigGroups)[MAX_PAGE_GROUPS], const PageConfig (& pageConfigs)[MAX_PAGES], int numGroups, int numPages )
    {
        memcpy(hdr.pageConfigGroups, pageConfigGroups, numGroups * sizeof(PageConfigGroup));
        hdr.numGroups = numGroups;
         memcpy(hdr.pageConfigs, pageConfigs, numPages * sizeof(PageConfig));
        hdr.numPages = numPages;
    }
   // std::list<PageAccessRecord> GetPageAccessRecords(std::string token);
    std::stringstream GetRecords(char *  token);
};


typedef struct
{
    union
    {
        FileConfigHdr1 fileConfigHdr;
        unsigned char buff[FILE_CONFIG_SIZE];
    };

}FileConfigHdrRec;






class FileCfg
{




    struct IndexRecord
    {
        long offset, len;
        long owner;
    };
    struct IndexConfig
    {
        std::string name;
    };

    struct MapRecord : public IndexRecord
    {
        bool writeProtect;
        time_t lastChange;
    };

    struct LockRecord : public IndexRecord
    {

    };

    struct SnapshotRecord : public IndexRecord
    {
        time_t time_taken;

    };
public:
    long pageSize;
    ei::MapIO mapio;
    FileConfigHdr1 *pHdr;





public:
    FileCfg();
    ~FileCfg();

    void Create( std::string filename, FileConfigHdr1 & hdr);


    void Open( std::string filename);
    void Close();

    void InitFile(std::string file,
                  long pageSize = PAGESIZE,
                  long maxNumMaps = MAX_MAPS,
                  long maxNumLocks = MAX_LOCKS,
                  long maxNumSnapshots = MAX_SNAPSHOTS);

};

#endif // INDEX_H
