//
// Created by gavan hood on 16/04/2015.
//

#include "IO.h"

namespace  ei {

    long IO::Open(std::string file, long pageSize) {
        return 0;
    }

    char * IO::Map(long offset, long len, bool manage) {
        return nullptr;
    }

    void IO::Sync() {

    }

    void IO::Close() {

    }


    long IO::getHeaderSize() const {
        return headerSize;
    }

    void IO::setHeaderSize(long headerSize) {
        IO::headerSize = headerSize;
    }

    long IO::getPageSize() const {
        return pageSize;
    }

    void IO::setPageSize(long pageSize) {
        IO::pageSize = pageSize;
    }

    long IO::getFd() const {
        return fd;
    }

    void IO::setFd(long fd) {
        IO::fd = fd;
    }

    time_t IO::getSyncFreq() const {
        return syncFreq;
    }

    void IO::setSyncFreq(time_t syncFreq) {
        IO::syncFreq = syncFreq;
    }

    void IO::UnMap(void * map, long len) {

    }
}
