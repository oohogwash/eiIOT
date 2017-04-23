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
unsigned char * serUChar( unsigned char * msg, unsigned char value)
{
    *msg++ = value;
    return msg;
}

unsigned char * serInt16(unsigned char * msg, int16_t value)
{
   // *msg++ = 2; // length
    I16CH ich;
    ich.ui16 = htons(value);
    memcpy(msg, ich.ch, 2 );
    return msg+2;
}
unsigned char * serUInt16(unsigned char * msg, uint16_t value)
{
   // *msg++ = 2; // length
    I16CH ich;
    ich.i16 = htons(value);
    memcpy(msg, ich.ch, 2 );
    return msg+2;
}
unsigned char * serInt32(unsigned char * msg, int32_t value)
{
   // *msg++ = 4; // length
    I32CH ich;
    ich.i32 = htonl(value);
    memcpy(msg, ich.ch, 4 );
    return msg+4;
}

unsigned char * serInt64(unsigned char * msg, int64_t value)
{
  //  *msg++ = 8; // length
    I64CH ich;
#ifdef NOT_ARDUINO
#ifndef _WIN
    ich.i64 = htonll(value);
#endif
    #endif
    memcpy(msg, ich.ch, 8 );
    return msg+8;
}
unsigned char * serFloat(unsigned char * msg, float value, char precision)
{
    *msg++ = precision; // precision
    int64_t lval = value * precision;
    msg = serInt64(msg, lval);
    return msg;
}
unsigned char * serDouble(unsigned char * msg, double value, char precision)
{
    *msg++ = precision; // precision
    int64_t lval = value * precision;
    msg = serInt64(msg, lval);
    return msg;
}


unsigned char * serSmallCharArr(unsigned char * msg, char * value, unsigned char len)
{
    //max length 255
    *msg++ = len; // length
    memcpy(msg, value, len);
    return msg+len;
}

unsigned char * serSmallString(unsigned char * msg, char * value)
{
    int len = strlen(value);
    if(len >255)
    {
       // <<to do add error>>
    }
    return serSmallCharArr(msg, value, len);
}

unsigned char * serCharArr(unsigned char * msg, char * value, uint16_t len)
{
    msg =  serInt16(msg, len); // length
    memcpy(msg, value, len);
    return msg+len;
}

unsigned char * serString(unsigned char * msg, char * value)
{
    return serCharArr(msg, value, strlen(value));
}

unsigned char * deserUChar(unsigned char * msg, unsigned char *value)
{
    *value = *msg++;
    return msg;
}

unsigned char * deserInt16(unsigned char * msg, int16_t  * value)
{
  //  msg++; // length should be 2
    I16CH ich;
    memcpy( ich.ch, msg, 2 );
    msg+=2;
    *value = ntohs(ich.i16);
    return msg;
}

unsigned char * deserUInt16(unsigned char * msg, uint16_t  * value)
{
   // msg++; // length should be 2
    I16CH ich;
    memcpy( ich.ch, msg, 2 );
    msg+=2;
    *value = ntohs(ich.ui16);
    return msg;
}


unsigned char * deserInt32(unsigned char * msg, int32_t * value)
{
   // msg++; // length should be 4
    I32CH ich;
    memcpy(ich.ch, msg, 4 );
    *value = htonl(ich.i32);
    return msg+4;
}

unsigned char * deserInt64(unsigned char * msg, int64_t  * value)
{
   // msg++ ; // length is always 8 so ignor
    I64CH ich;
    memcpy(ich.ch, msg, 8 );
    #ifdef NOT_ARDUINO
#ifndef _WIN
    *value = htonll(ich.i64);
#endif
    #endif

    return msg+8;
}


unsigned char * deserFloat(unsigned char * msg, float * value)
{
    int precision = *msg++; // precision
    I64CH lval;
    msg = deserInt64(msg, &lval.i64);
    *value = lval.i64 / precision;
    return msg;
}
unsigned char * deserDouble(unsigned char * msg, double * value)
{
    int precision = *msg++; // precision
    I64CH lval;
    msg = deserInt64(msg, &lval.i64);
    *value = lval.i64 / precision;
    return msg;
}


unsigned char * deserSmallCharArr(unsigned char * msg, char * value, unsigned char  * len)
{
    //max length 255
    *len = *msg++ ; // length
    memcpy(value, msg, *len);
    return msg+*len;
}

unsigned char * deserSmallString(unsigned char * msg, char * value, unsigned char * length)
{
    unsigned char len;
    msg = deserSmallCharArr(msg, value, &len);
    value[len]=0;
    if(length != 0)
    {
        *length = len;
    }
    return msg;
}

unsigned char * deserCharArr(unsigned char * msg, char * value, uint16_t * len)
{
    msg =  deserUInt16(msg, len); // length
    memcpy(value, msg,  *len);
    return msg+*len;
}

unsigned char * deserString(unsigned char * msg, char * value, uint16_t * length)
{
   uint16_t len;
   msg = deserCharArr(msg, value, &len);
   value[len] = 0;
   if(length != 0)
   {
       *length = len;
   }
   return msg;
}







msgDef::msgDef()
{

}
}
