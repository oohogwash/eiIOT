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


class eiString
{
    static const int MAX_LEN = 256;
    char value[MAX_LEN];
public:
    eiString();
    int deserialize( unsigned char ** msg);
    int serialize(unsigned char ** msg);
};

class eiInt
{
    int32_t value;
public:
    eiInt();
    int deserialize( unsigned char ** msg);
    int serialize(unsigned char ** msg);
};

class eiDouble
{
    double value;
    int precision;
public:
    eiDouble();
    int deserialize( unsigned char ** msg);
    int serialize(unsigned char ** msg);
};




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

int serUChar( unsigned char ** msg, unsigned char value);
int serInt16(unsigned char ** msg, int16_t value);
int serUInt16(unsigned char ** msg, uint16_t value);
int serInt32(unsigned char ** msg, int32_t * value);
int serInt32Arr(unsigned char ** msg, int32_t * value, unsigned char cnt = 1);
int serInt64(unsigned char ** msg, int64_t value);
int serFloat(unsigned char ** msg, float value, char precision);
int serDouble(unsigned char ** msg, double value, char precision);
int serSmallCharArr(unsigned char ** msg, char * value, unsigned char len);
int serCharArr(unsigned char ** msg, char * value, uint16_t len);
int serString(unsigned char ** msg, char * value);
int serSmallString(unsigned char ** msg, char * value);

int deserUChar( unsigned char ** msg, unsigned char *value);
int deserInt16( unsigned char ** msg, int16_t  * value);
int deserUInt16( unsigned char ** msg, uint16_t  * value);
int deserInt32( unsigned char ** msg, int32_t * value);
int deserInt32Arr( unsigned char ** msg, int32_t * value, unsigned char * len);
int deserInt64( unsigned char ** msg, int64_t  * value);
int deserFloat( unsigned char ** msg, float * value);
int deserDouble( unsigned char ** msg, double * value);
int deserSmallCharArr( unsigned char ** msg, char * value, unsigned char  * len);
int deserCharArr( unsigned char ** msg, char * value, uint16_t * len);
int deserString( unsigned char ** msg, char * value, uint16_t * length=0);
int deserSmallString( unsigned char ** msg, char * value, unsigned char * length=0);



}
#endif // MSGDEF_H
