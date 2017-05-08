//
// Created by gavan hood on 19/04/2015.
//

#ifndef TEST_MAPIO_H
#define TEST_MAPIO_H

#include "IO.h"

namespace  ei {

    class MapIO : IO {
    public:
        virtual void UnMap(void * map , long len) override;

        virtual char *Map(long offset, long len, bool manage = true) override;

        virtual long Open(std::string file, long pageSize) override;

        virtual void Sync() override;

        virtual void Close() override;
    };
}

#endif //TEST_MAPIO_H
