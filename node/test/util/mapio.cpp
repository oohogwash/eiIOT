//
// Created by gavan hood on 19/04/2015.
//



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <list>

#include "MapIO.h"

namespace  ei
{

#define FILEPATH "/tmp/mmapped.bin"
#define NUMINTS  (1000)
#define FILESIZE (NUMINTS * sizeof(int))



    long MapIO::Open(std::string file, long pageSize)
    {

        long result = 0;
        if(fd == UNSET) {
            long pgsz = sysconf(_SC_PAGESIZE);
            this->pageSize = pgsz > pageSize ? pgsz : ((pageSize / pgsz) + 1) * pgsz;
            long result = 0;
         /* Open a file for writing.
         *  - Creating the file if it doesn't exist.
         * Note: "O_WRONLY" mode is not sufficient when mmaping.
         */
            fd = open(file.c_str(), O_RDWR | O_CREAT, (mode_t) 0600);
            if (fd == UNSET) { // UNSET = -1
                perror("Error opening file for writing");
                return EXIT_FAILURE;
            }
        }
        return result;
    }

    void MapIO::Sync() {
        IO::Sync();
    }

    void MapIO::Close() {

        for (MAPINFO * pmi : maps) {
            UnMap(pmi->map, pmi->len);
            delete(pmi);
        }
        maps.clear();
        close(fd);
        fd = UNSET;
    }

    char *MapIO::Map(long offset, long len, bool manage) {
        long result = 0;
        /* Stretch the file size to the size of the (mmapped) array of ints
        */
        struct stat stat;
        fstat(fd, &stat);

        if (stat.st_size < offset+ len) {
            result = lseek(fd, offset + len - 1, SEEK_SET);
            if (result == -1) {
                close(fd);
                perror("Error calling lseek() to 'stretch' the file");
                return nullptr;
            }

            /* Something needs to be written at the end of the file to
         * have the file actually have the new size.
         * Just writing an empty string at the current file position will do.
         *
         * Note:
         *  - The current position in the file is at the end of the stretched
         *    file due to the call to lseek().
         *  - An empty string is actually a single '\0' character, so a zero-byte
         *    will be written at the last byte of the file.
         */
            result = write(fd, "", 1);
            if (result != 1) {
                close(fd);
                perror("Error writing last byte of the file");
                return nullptr;
            }
        }

        /* Now the file is ready to be mmapped.
         */
        char * map = (char *) mmap(0, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
        if (map == MAP_FAILED) {
            close(fd);
            perror("Error mmapping the file");
            return nullptr;
        }
        else if (manage) //a managed map will be automaticaly removed
        {

            MAPINFO * pmi = new MAPINFO;
            pmi->len = len;
            pmi->map = map;
            maps.push_back(pmi);
        }

        return map;
    }

    void MapIO::UnMap(void * map, long len) {
        if(fd != UNSET) {
            /* Don't forget to free the mmapped memory
   */
            if (munmap(map, len) == -1) {
                perror("Error un-mmapping the file");
                /* Decide here whether to close(fd) and exit() or not. Depends... */
            }


        }
    }
}
