#include "osheader.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "msgdef.h"

namespace eiCom {


#ifndef NOT_ARDUINO

#define htons(x) ( ((x)<< 8 & 0xFF00) | \
                   ((x)>> 8 & 0x00FF) )
#define ntohs(x) htons(x)

#define htonl(x) ( ((x)<<24 & 0xFF000000UL) | \
                   ((x)<< 8 & 0x00FF0000UL) | \
                   ((x)>> 8 & 0x0000FF00UL) | \
                   ((x)>>24 & 0x000000FFUL) )
#define ntohl(x) htonl(x)

#define ntohll(x) ntohll(x)
//#define htonll(x) htonll(x)

#endif


eiInt::eiInt()
{

}

int eiInt::deserialize( unsigned char ** msg)
{
    return deserInt32(msg, &value);
}

int eiInt::serialize(unsigned char ** msg)
{
    serInt32(msg, &value);
}


eiDouble::eiDouble()
{
    precision = 5;
}

int eiDouble::deserialize( unsigned char ** msg)
{
    return deserDouble(msg, &value);
}

int eiDouble::serialize(unsigned char ** msg)
{
    return serDouble(msg, value, precision);
}


eiString::eiString()
{

}

int eiString::deserialize( unsigned char ** msg)
{
    return deserSmallString(msg, value);
}

int eiString::serialize(unsigned char ** msg)
{
    return serSmallString(msg, value);
}





int strcpyn(char * dest, const int destlen, const char * source)
{
    int sourcelen = strlen(source);
    int lcopy = fmin(destlen, sourcelen);
    memcpy(dest, source, lcopy);
    dest[lcopy] = 0;
    return lcopy;
}

int memcpyn(char *dest, const int destlen, const char * source, const int sourcelen, const bool addnullterminator)
{
    int lcopy = fmin(destlen, sourcelen) ;
    memcpy(dest, source, lcopy);
    if(addnullterminator)
    {
        dest[lcopy]=0;
    }
    return lcopy;
}

char * memcpyn1(char *dest, int destlen, char * source, int sourcelen, bool addnullterminator)
{
    int lcopy = fmin(destlen, sourcelen) ;
    memcpy(dest, source, lcopy);
    if(addnullterminator)
    {
        dest[lcopy]=0;
    }
    return &dest[lcopy];
}


char * memcpyn2(char *dest, char * source, int len)
{
    memcpy(dest, source, len);
    return &dest[len];
}

unsigned char * memcpyn2(unsigned char *dest, char * source, int len)
{
    memcpy(dest, source, len);
    return &dest[len];
}
int serUChar( unsigned char ** msg, unsigned char value)
{
    **msg = value;
    (*msg)++;
    return 1;
}

int serInt16(unsigned char ** msg, int16_t value)
{
   // *msg++ = 2; // length
    I16CH ich;
    ich.ui16 = htons(value);
    memcpy(*msg, ich.ch, 2 );
    (*msg)+=2;
    return 2;
}


int serUInt16(unsigned char * * msg, uint16_t value)
{
    I16CH ich;
    ich.i16 = htons(value);
    memcpy(*msg, ich.ch, 2 );
    (*msg)+=2;
    return 2;
}

int serInt32(unsigned char ** msg, int32_t * value)
{
    I32CH ich;
    ich.i32 = htonl(*value++);
    memcpy(*msg, ich.ch, 4 );
    *msg+=4;
    return 4;
}

int serInt32Arr(unsigned char ** msg, int32_t * value, unsigned char cnt)
{
   int sz= serUChar(msg, cnt);
   while(cnt-- > 0){
       sz += serInt32(msg, value++);
    };
    return sz;
}

int serInt64(unsigned char ** msg, int64_t value)
{
    I64CH ich;
#ifdef NOT_ARDUINO
#ifndef _WIN
    ich.i64 = htonll(value);
#endif
    #endif
    memcpy(*msg, ich.ch, 8 );
    return 8;
}
int serFloat(unsigned char ** msg, float value, char precision)
{
    int sz = serUChar(msg, precision);
    int64_t lval = value * precision;
    sz += serInt64(msg, lval);
    return sz;
}
int serDouble(unsigned char ** msg, double value, char precision)
{
    int sz = serUChar(msg, precision);
    int64_t lval = value * precision;
    sz+=serInt64(msg, lval);
    return sz;
}


int serSmallCharArr(unsigned char ** msg, char * value, unsigned char len)
{
    //max length 255
    int sz = serUChar(msg, len);
    memcpy(*msg, value, len);
    (*msg) += len;
    return sz+len;
}

int serSmallString(unsigned char ** msg, char * value)
{
    int len = strlen(value);
    if(len >255)
    {
       // <<to do add error>>
    }
    return serSmallCharArr(msg, value, len);
}

int serCharArr(unsigned char ** msg, char * value, uint16_t len)
{
    int sz = serInt16(msg, len); // length
    memcpy(*msg, value, len);
    (*msg)+=len;
    return sz+len;
}

int serString(unsigned char ** msg, char * value)
{
    return serCharArr(msg, value, strlen(value));
}

int deserUChar( unsigned char ** msg, unsigned char *value)
{
    *value = **msg;
    (*msg)++;
    return 1;
}

int deserInt16( unsigned char ** msg, int16_t  * value)
{
    I16CH ich;
    memcpy( ich.ch, *msg, 2 );
    (*msg)+=2;
    *value = ntohs(ich.i16);
    return 2;
}

int deserUInt16( unsigned char ** msg, uint16_t  * value)
{
    I16CH ich;
    memcpy( ich.ch, *msg, 2 );
    (*msg)+=2;
    *value = ntohs(ich.ui16);
    return 2;
}
int deserInt32( unsigned char ** msg, int32_t * value)
{
    I32CH ich;
    memcpy(ich.ch, *msg, 4 );
    *value = htonl(ich.i32);
    (*msg)+=4;
    return 4;
}

int deserInt32Arr( unsigned char ** msg, int32_t * value, unsigned char * cnt)
{
   int sz = deserUChar(msg, cnt);
   unsigned char num = *cnt;
   while(num-- >0)
    {
       sz += deserInt32(msg, value++);
    };
    return sz;
}

int deserInt64( unsigned char ** msg, int64_t  * value)
{
    I64CH ich;
    memcpy(ich.ch, *msg, 8 );
    #ifdef NOT_ARDUINO
#ifndef _WIN
    *value = htonll(ich.i64);
#endif
    #endif
    (*msg)+=8;
    return 8;
}


int deserFloat( unsigned char ** msg, float * value)
{
    unsigned char precision=0;
    int sz = deserUChar(msg, &precision); // precision
    I64CH lval;
    sz += deserInt64(msg, &lval.i64);
    *value = lval.i64 / precision;
    return sz;
}
int deserDouble( unsigned char ** msg, double * value)
{
    unsigned char precision=0;
    int sz = deserUChar(msg, &precision); // precision
    I64CH lval;
    sz += deserInt64(msg, &lval.i64);
    *value = lval.i64 / precision;
    return sz;
}


int deserSmallCharArr( unsigned char ** msg, char * value, unsigned char  * len)
{
    //max length 255
   //*len = *msg++ ; // length
    int sz = deserUChar(msg, len);
    memcpy(value, *msg, *len);
    (*msg) += *len;
    return sz+*len;
}

int deserSmallString( unsigned char ** msg, char * value, unsigned char * length)
{
    unsigned char len;
    int sz = deserSmallCharArr(msg, value, &len);
    value[len]=0;
    if(length != 0)
    {
        *length = len;
    }
    return sz;
}

int deserCharArr( unsigned char ** msg, char * value, uint16_t * len)
{
    int sz =  deserUInt16(msg, len); // length
    memcpy(value, msg,  *len);
    (*msg) += *len;
    return sz+*len;
}

int deserString( unsigned char ** msg, char * value, uint16_t * length)
{
   uint16_t len;
   int sz = deserCharArr(msg, value, &len);
   value[len] = 0;
   if(length != 0)
   {
       *length = len;
   }
   return sz;
}







msgDef::msgDef()
{

}
}
