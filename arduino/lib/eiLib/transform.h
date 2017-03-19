#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum transformType : int
{
   mt_Undefined = 0,
   mt_MAP,
   mt_Formulae,
   mt_Linear,
   mt_BoolMin,
   mt_BoolMax
}MT;

#ifndef min
#define min(a,b) \
  ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })
#endif

#ifndef strndup
char * strndup(const char * buffer, int maxlen)
{
  int len = min(maxlen, strlen(buffer) ) +1;
  char * str = new char[len];
  strncpy(str, buffer, len);
  str[len] = NULL;

  return str;
}
#endif


#ifndef SAFEDELETE

#define SAFEDELETE(a) if(a != NULL){ delete a; a = NULL;}

#endif // SAFEDELETE



class Transform
{
   const int nan = -32000;
   char * ID;
   char * warnTopic = NULL;
   char * errTopic=NULL;
   char * outTopic=NULL;
   int * inputs;
   int * outputs;
public:
    int ver;
    int szIOMap;
    int min;
    int max;
    int lowarn, hiwarn;
    int lowerr,hierr;
    transformType type;
public:
    Transform();
    ~Transform();
    Transform( char * ID, int ver);
    void setID(char * id);
    void setTopics(char * warn, char * err, char * out);
    void setInputs (int * inputs);
    void setOutputs (int * outputs);
    void serialize(char * buffer);
    char * loadItem(const char *buffer,  const char * id);
    void loadStr(char * buffer, char ** item, char * id);
    void loadType(char * buffer, MT * item, char * id);
    void loadInt(char * buffer, int * item, char * id);
    void loadIntArray(char * buffer, int ** ints, char * id);
    void deserialize(char * buffer);
    void addJSONItem(char * buffer, const char * id,  char * item, bool addComma = true);
    void addJSONItem(char * buffer, const char * id,  int item, bool addComma = true);
    void addJSONItem(char * buffer, const char * id,  int * items, int count, bool addComma = true);
};





class Transforms
{
    Transform ** items;
    int size;
    bool owned;
    void cleanup();

    int count(const char * buffer, const char * text);

    typedef struct el
    {
        const char * start=NULL;
        const char * end=NULL;
        int length(){ return end-start;}
        char * element()
        {
            return strndup(start, length());
        }

    }EL;
    class Elements
    {
    public:
        EL ** elements;
        int len;

        Elements(int len)
        {
            this->len = len;
            elements = new EL * [len];
            for(int i = 0; i < len; i++)
            {
               elements[i] = new EL;
            }

        }
        ~Elements()
        {
            for(int i = 0; i < len; i++)
            {
                SAFEDELETE(elements[i]);
            }
        }
    };
    const char * matchclosingbrace(const char * start);
    Elements elements(const char * buffer , const char * text);

public:
    Transforms();
    Transforms(int size);
    void create(int size);

    Transforms(Transform  ** t, int size);
    void serialize(char * buffer);

    void deserialize(char * buffer);

};












#endif // TRANSFORM_H
