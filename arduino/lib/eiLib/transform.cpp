#include "transform.h"


//const int MAX_MAPS = 10;
//int map[MAX_MAPS+1];



/*
{
  "transform" : {
    "ID":"",
    "type":"",
    "min":"",
    "max":"",
    "inputs":[""],
    "outputs":[""],
    "low_warn":"",
    "hi_warn":"",
    "low_err":"",
    "hi_err":"",
    "warn_pin":"",
    "err_pin":"",
    "warn_topic":"",
    "err_topic":""
  }
}
*/




Transform::Transform()
{
    ver = 1;
    ID = NULL;
    type = mt_Undefined;
    inputs = NULL;
    outputs = NULL;
    min=nan;
    max=nan;
    lowarn=nan;
    hiwarn=nan;
    lowerr = nan;
    hierr = nan;
}

Transform::~Transform()
{
    SAFEDELETE(ID);
    SAFEDELETE(warnTopic);
    SAFEDELETE(errTopic);
    SAFEDELETE(outTopic);
    SAFEDELETE(inputs);
    SAFEDELETE(outputs);
}



Transform::Transform( char * ID, int ver)
{
   this->ver = ver;
   this->ID = strdup(ID);
}

void Transform::setID(char * id)
{
    ID = strdup(id);
}
void Transform::setTopics(char * warn, char * err, char * out)
{
    warnTopic = strdup(warn);
    errTopic = strdup(err);
    outTopic = strdup(out);
}
void Transform::setInputs (int * inputs)
{
    this->inputs = new int [sizeof(inputs)];
    for(int idx = 0; idx < sizeof(inputs); idx++)
    {
        this->inputs[idx] = inputs[idx];
    }
}
void Transform::setOutputs (int * outputs)
{
    this->outputs = new int [sizeof(outputs)];
    for(int idx = 0; idx < sizeof(outputs); idx++)
    {
        this->outputs[idx] = outputs[idx];
    }
}
void Transform::serialize(char * buffer)
{
    strcpy(buffer,"{\"transform\":{\n");
    addJSONItem(buffer, "ver", ver);
    addJSONItem(buffer, "ID", ID);
    addJSONItem(buffer, "type", type);
    addJSONItem(buffer, "szIOMap", szIOMap);
    addJSONItem(buffer, "inputs", inputs, szIOMap);
    addJSONItem(buffer, "outputs", outputs, szIOMap);
    addJSONItem(buffer, "min", min);
    addJSONItem(buffer, "max", max);
    addJSONItem(buffer, "lowarn", lowarn);
    addJSONItem(buffer, "hiwarn", hiwarn);
    addJSONItem(buffer, "lowerr", lowerr);
    addJSONItem(buffer, "hierr", hierr);
    addJSONItem(buffer, "warnTopic", warnTopic);
    addJSONItem(buffer, "errTopic", errTopic);
    addJSONItem(buffer, "outTopic", outTopic, false);
    strcat(buffer,"}\n}\n");
}

char * Transform::loadItem(const char *buffer,  const char * id)
{
  static char buff[128];
  buff[0]=NULL;
  sprintf(buff,"\"%s\":", id);
  char * ptr = strstr(buffer,buff);
  if(ptr != NULL)
  {
      ptr+= strlen(buff)+1;
      char *pstart = ptr;
      char *pend = strchr(pstart+1, '\"');
      strncpy(buff, pstart, pend-pstart);
      buff[pend-pstart] = NULL;
      return buff;
  }
  return NULL;
}

void Transform::loadStr(char * buffer, char ** item, char * id)
{
     char * ptr = loadItem(buffer,id);
     if(ptr != NULL)
     {
        *item = strdup(ptr);
     }
     else
     {
         *item = NULL;
     }
}



void Transform::loadType(char * buffer, MT * item, char * id)
{
    *item = mt_Undefined;
    char *ptr = loadItem(buffer, id);
    if(ptr != NULL)
    {
        switch(atoi(ptr)){
         case 0:
            *item = mt_Undefined;
            break;
         case 1:
            *item = mt_MAP;
            break;
        case 2:
           *item = mt_Formulae;
           break;
        case 3:
           *item = mt_Linear;
           break;
        case 4:
           *item = mt_BoolMin;
           break;
        case 5:
           *item = mt_BoolMax;
           break;
        default:
            *item = mt_Undefined;
        }
    }
}

void Transform::loadInt(char * buffer, int * item, char * id)
{
    char * ptr = loadItem(buffer, id);
    if( ptr != NULL)
    {
        *item = atoi(ptr);
    }
}


void Transform::loadIntArray(char * buffer, int ** ints, char * id)
{
    char * ptr = loadItem(buffer, id);
    if(ptr != NULL)
    {
        ptr = strdup(ptr);
        int count = 0;
        char* pTemp=ptr;
        while(pTemp != NULL)
        {
            pTemp = strchr(pTemp, ',');
            if( pTemp ) {
                pTemp++;
                count++;
            }
        }
        *ints = new int[count];
        int * intarr = *ints;
        count = 0;
        char * token = strtok(ptr, ",");
        while( token != NULL )
        {
            char * ptr1 = strchr(token, ']');
            if(ptr1 != NULL)
            {
                *ptr1 = NULL;
                intarr[count] = atoi(token);
                break;
            }
            intarr[count++] = atoi(token);
            token = strtok(NULL, ",");
       }
    }
}
void Transform::deserialize(char * buffer)
{
    loadInt(buffer, &ver, "ver");
    loadStr(buffer, &ID,"ID");
    loadType(buffer, &type, "type");
    loadInt(buffer, &szIOMap, "szIOMap");
    loadIntArray(buffer, &inputs, "inputs");
    loadIntArray(buffer, &outputs, "outputs");
    loadInt(buffer, &min,"min");
    loadInt(buffer, &max,"max");
    loadInt(buffer, &lowarn,"lowarn");
    loadInt(buffer, &hiwarn,"hiwarn");
    loadInt(buffer, &lowerr,"lowerr");
    loadInt(buffer, &hierr,"hierr");
    loadStr(buffer, &warnTopic,"warnTopic");
    loadStr(buffer, &errTopic,"errTopic");
    loadStr(buffer, &outTopic,"outTopic");

}


void Transform::addJSONItem(char * buffer, const char * id,  char * item, bool addComma)
{
    if( item != NULL)
    {
        char buff[128];
        sprintf(buff, "\"%s\":\"%s\"%s", id,item, addComma?",\n":"\n");
        strcat(buffer, buff);
    }
}
void Transform::addJSONItem(char * buffer, const char * id,  int item, bool addComma)
{
    if(item != nan)
    {
        char buff[128];
        sprintf(buff, "\"%s\":\"%d\"%s", id,item, addComma?",\n":"\n");
        strcat(buffer, buff);
    }
}
void Transform::addJSONItem(char * buffer, const char * id,  int * items, int count, bool addComma)
{
    if(items != NULL && count > 0)
    {
        char buff[128];
        sprintf(buff, "\"%s\":[", id);
        strcat(buffer, buff);
        for(int * item = items; item-items < count; item++)
        {
            sprintf(buff,"%d%s", *item, item-items < count-1 ? ",":"");
            strcat(buffer, buff);
        }
        sprintf(buff, "]%s", addComma?",\n":"\n");
        strcat(buffer, buff);
    }
}


void Transforms::cleanup()
    {
        if( owned)
        {
            for(int idx = 0; idx < size; idx++)
            {
                SAFEDELETE(items[idx]);
            }
            SAFEDELETE(items);
        }
        items=NULL;
        size = 0;
    }
int Transforms::count(const char * buffer, const char * text)
{
    int idx = 0;
    const char * ptr = buffer;
    const char * ptr1;
    int len = strlen(text);
    while((ptr1 = strchr(ptr, text[0])) != NULL)
    {
        if(strncmp(ptr1, text, len) == 0)
        {
            idx++;
        }
        ptr = ptr1 +1;
    }
    return idx;
}



const char * Transforms::matchclosingbrace(const char * start)
{
    int match = 1; //starting on {
    do
    {
        start++;
        switch(*start)
        {
          case '{':
            match++;
            break;
          case '}':
            match --;
            break;
        }
    }while( match > 0 && *start != NULL);
    return start;
}

Transforms::Elements Transforms::elements(const char * buffer , const char * text)
{
    Elements output(count(buffer, text));
    int idx =0;
    int len = strlen(text);
    const char * ptr = buffer;
    const char * ptr1, * ptrlast;

    if(output.len > 0)
    {
        while((ptr1 = strchr(ptr, text[0])) != NULL)
        {
            if(strncmp(ptr1, text, len) == 0)
            {

                if(idx > 0)
                { // this assumes the previous element terminates the next element with a , before and no whitespace
                    //const char * last = strrchr(ptr )  use match closing brace instead
                    output.elements[idx-1 ]->end = matchclosingbrace(output.elements[idx -1]->start) +1;// ptr1  -1;
                    output.elements[idx++]->start = ptr1;
                }
                else
                {
                    output.elements[idx++]->start = ptr1;
                }
                 ptrlast = ptr;
            }

            ptr = ptr1 +1;
        }

        output.elements[idx -1]->end = matchclosingbrace(output.elements[idx -1]->start) +1;
    }
    return output;
}


Transforms::Transforms()
{
    items = NULL;
    size = 0;
    owned = true;
}
Transforms::Transforms(int size)
{
    create(size);
}
void Transforms::create(int size)
{
    this->size=size;
    items = new Transform * [size];
    for(int idx = 0; idx < size; idx ++)
    {
        items[idx] = new Transform();
    }
    owned=true;
}

Transforms::Transforms(Transform  ** t, int size)
{
    items = t;
    this->size = size;
    owned=false;
}
void Transforms::serialize(char * buffer)
{
    sprintf(buffer,"{\"Transforms\":[");
    char buff[1024];
    for(Transform ** t = items; t -items < size; t++)
    {
       (*t)->serialize(buff);
        if(t-items < size -1)
        {
            strcat(buff,",");
        }
        strcat(buffer, buff);
    }
    strcat(buffer,"]}");

}


void Transforms::deserialize(char * buffer)
{
    cleanup();


    Elements e = elements(buffer, "{\"transform\"");
    create(e.len);

    for( int idx = 0; idx < e.len; idx++)
    {
        items[idx]->deserialize(e.elements[idx]->element());
    }
}



