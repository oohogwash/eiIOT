#ifndef COLLECTION_H
#define COLLECTION_H
#include <inttypes.h>
#include "createableobject.h"

namespace eiMsg
{






class Collection
{
    eiKernel::CreateableObject ** objects ;
    int16_t objectsLen;
    static const int MAXCOLLECTIOMDATALEN = 1024;
public:
    unsigned char collection[MAXCOLLECTIOMDATALEN+1];
    int collectionLen;
    Collection();
    int serialize(unsigned char ** msg);
    int deserialize ( unsigned char ** msg);
    int serialize (unsigned char ** msg, eiKernel::CreateableObject * objects, int objectLen);


};



}
#endif // COLLECTION_H
