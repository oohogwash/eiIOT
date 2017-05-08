#include "log.h"
#include "syslog.h"
#include "easylogging++.h"
#include <memory>
#include <iostream>
#include <string>
#include <cstdio>
#include "cxutils/cxutils.h"


using namespace std;
using namespace CxUtils;



#include <iostream>
using namespace std;

INITIALIZE_EASYLOGGINGPP

el::Logger* _eiLogger = el::Loggers::getLogger("default");

Mutex _logMutex;

namespace ei
{
namespace core
{
namespace diag
{
template<typename ... Args>
string string_format( const char* format, Args ... args )
{
    size_t size = snprintf( nullptr, 0, format, args ... ) + 1; // Extra space for '\0'
    unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format, args ... );
    return string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}



void eiFatal( const char * fmt, ...)
{

    va_list vl;

    va_start(vl, fmt);
    int size = vsnprintf(0, 0, fmt, vl) + sizeof('\0');
    va_end(vl);

    char buffer[size];

    va_start(vl, fmt);
    size = vsnprintf(buffer, size, fmt, vl);
    va_end(vl);
    _eiLogger->fatal((char *)buffer);
}

void eiError( const char * fmt,...)
{
    va_list vl;

    va_start(vl, fmt);
    int size = vsnprintf(0, 0, fmt, vl) + sizeof('\0');
    va_end(vl);

    char buffer[size];

    va_start(vl, fmt);
    size = vsnprintf(buffer, size, fmt, vl);
    va_end(vl);
    _eiLogger->error((char *)buffer);
}

void eiLog( const char * fmt, ...)
{
    _logMutex.Lock();
    va_list vl;

    va_start(vl, fmt);
    int size = vsnprintf(0, 0, fmt, vl) + sizeof('\0');
    va_end(vl);

    char buffer[size];

    va_start(vl, fmt);
    size = vsnprintf(buffer, size, fmt, vl);
    va_end(vl);
    _eiLogger->info((char *)buffer);
    _logMutex.Unlock();
}

void eiDebug(  const char* fmt, ...)
{
     _logMutex.Lock();

    va_list vl;

    va_start(vl, fmt);
    int size = vsnprintf(0, 0, fmt, vl) + sizeof('\0');
    va_end(vl);

    char buffer[size];

    va_start(vl, fmt);
    size = vsnprintf(buffer, size, fmt, vl);
    va_end(vl);
    _eiLogger->debug((char *)buffer);
    _logMutex.Unlock();

}










/*None = 0,
Critical,
Error,
Warning,
Info,
Debug
*/

void eiLog(diagLevel level, char * fmt, ...)
{
    if( level  > Level::Debug)
    {
        CLOG(DEBUG, "") << fmt << endl;
    }
    else if (level > Level::Info)
    {

    }
}


void InitLogger()
{

    ELPP_INITIALIZE_SYSLOG("Triple", LOG_PID | LOG_CONS | LOG_PERROR, LOG_USER);

    el::Configurations defaultConf;
    defaultConf.setToDefault();
  //  defaultConf.setGlobally(el::ConfigurationType::Format, "%level  %datetime  %func %file %line %msg");
    defaultConf.setGlobally(el::ConfigurationType::Format, "%level  %datetime  %func %msg");
    defaultConf.setGlobally(el::ConfigurationType::ToFile, "true");
    defaultConf.setGlobally(el::ConfigurationType::Filename, "/tmp/triple.log");
    el::Loggers::reconfigureLogger("default", defaultConf);
}



void Log::Debug(char * fmt)
{
 LOG(INFO) << fmt << endl;
}

Log::Log()
{
//el::Logger* defaultLogger = el::Loggers::getLogger("default");
  //  el::Configurations * pcfg = _eiLogger->configurations();
  //  pcfg["default"].
}

Log::~Log()
{

}

}
}
}
