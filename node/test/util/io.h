//
// Created by gavan hood on 16/04/2015.
//

#ifndef TEST_IO_H
#define TEST_IO_H

#include <string>
#include <list>

namespace ei {


    class IO{

    public:

        virtual long Open(std::string file, long pageSize);

        virtual char * Map(long offset, long len, bool manage = true );

        virtual void UnMap(void * map, long len);

        virtual void Sync();

        virtual void Close();


        long getHeaderSize() const;

        void setHeaderSize(long headerSize);

        long getPageSize() const;

        void setPageSize(long pageSize);

        long getFd() const;

        void setFd(long fd);

        time_t getSyncFreq() const;

        void setSyncFreq(time_t syncFreq);


    protected:
        long UNSET = -1;
        long headerSize = 1024;
        long pageSize = 4094;
        long fd = UNSET;
        long offset = UNSET, len = UNSET;
        //void * map = nullptr;

        typedef struct mapInfo
        {
            void * map;
            long len;
        }MAPINFO;

        std::list<MAPINFO *> maps;

        time_t  syncFreq;
        enum enStorageMode
        {
            file,
            memMappedFile
        };
        enStorageMode mode = file;
    public:
        enStorageMode const &getMode() const {
            return mode;
        }

        void setMode(enStorageMode const &mode) {
            IO::mode = mode;
        }



       // long Write(char * buff, long len, )

    };

}
#endif //TEST_IO_H
