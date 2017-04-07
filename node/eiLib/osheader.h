#ifndef OSHEADER_H
#define OSHEADER_H

#define __MAC_OSX__


#if defined(__linux__) || defined(__FreeBSD__) || defined(__MAC_OSX__)

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/file.h>
#include <errno.h>

#else

#include <windows.h>

#endif




#endif // OSHEADER_H
