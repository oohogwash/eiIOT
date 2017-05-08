#ifndef LOG_H
#define LOG_H



namespace ei
{
namespace core
{
namespace diag
{
typedef enum diagLevel
{
  None = 0,
  Critical,
  Error,
  Warning,
  Info,
  Debug
}Level;

typedef enum diagOutput
{
    none = 0,
    file,
    console,
    dialog,
}DiagOutput;

void eiLog(diagLevel,  const char * fmt, ...);

void eiDebug( const char * fmt, ...);

void eiFatal( const char * fmt, ...);

void eiError( const char * fmt, ...);


void InitLogger();

class Log
{
public:
    Log();
    ~Log();
    void log(Level level,char * fmt, ...);
    void Debug(char * fmt);
};
}
}
}
#endif // LOG_H
