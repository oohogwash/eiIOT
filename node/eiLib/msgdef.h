#ifndef MSGDEF_H
#define MSGDEF_H
#include <inttypes.h>

namespace eiCom
{

class msgDef
{
public:
    msgDef();
};

int memcpyn(char *dest,  const int destlen, const char * source, const int sourcelen, const bool addNullTerminator=false);
int strcpyn(char * dest,  const int destlen, const char * source);


char * memcpyn2(char *dest, char * source, int len);
unsigned char * memcpyn2(unsigned char *dest, char * source, int len);


union I16CH
{
    char ch[2];
    unsigned char uch[2];
    int16_t i16;
    uint16_t ui16;
};

union I32CH
{
    char ch[4];
    unsigned char uch[4];
    int32_t i32;
    uint32_t ui32;
};
union I64CH
{
    char ch[8];
    unsigned char uch[8];
    int64_t i64;
    uint64_t ui64;
};

unsigned char * serUChar( unsigned char * msg, unsigned char value);
unsigned char * serInt16(unsigned char * msg, int16_t value);
unsigned char * serUInt16(unsigned char * msg, uint16_t value);
unsigned char * serInt32(unsigned char * msg, int32_t value);
unsigned char * serInt64(unsigned char * msg, int64_t value);
unsigned char * serFloat(unsigned char * msg, float value, char precision);
unsigned char * serDouble(unsigned char * msg, double value, char precision);
unsigned char * serSmallCharArr(unsigned char * msg, char * value, unsigned char len);
unsigned char * serCharArr(unsigned char * msg, char * value, uint16_t len);
unsigned char * serString(unsigned char * msg, char * value);
unsigned char * serSmallString(unsigned char * msg, char * value);

unsigned char * deserUChar(unsigned char * msg, unsigned char *value);
unsigned char * deserInt16(unsigned char * msg, int16_t  * value);
unsigned char * deserUInt16(unsigned char * msg, uint16_t  * value);
unsigned char * deserInt32(unsigned char * msg, int32_t * value);
unsigned char * deserInt64(unsigned char * msg, int64_t  * value);
unsigned char * deserFloat(unsigned char * msg, float * value);
unsigned char * deserDouble(unsigned char * msg, double * value);
unsigned char * deserSmallCharArr(unsigned char * msg, char * value, unsigned char  * len);
unsigned char * deserCharArr(unsigned char * msg, char * value, uint16_t * len);
unsigned char * deserString(unsigned char * msg, char * value, uint16_t * length=0);
unsigned char * deserSmallString(unsigned char * msg, char * value, unsigned char * length=0);



}
#endif // MSGDEF_H
