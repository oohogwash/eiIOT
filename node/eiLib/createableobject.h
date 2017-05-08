#ifndef CREATEABLEOBJECT_H
#define CREATEABLEOBJECT_H
namespace eiKernel {



class CreateableObject
{
public:
    static const unsigned char MAX_CLSID_LEN=255;
    char clsName[MAX_CLSID_LEN];
    unsigned char clsidLen;
    CreateableObject(){}
    CreateableObject(const char * clsName);
    int serialize(unsigned char ** msg);
    int deserialize(  unsigned char ** msg);
    void dump();
};
}

#endif // CREATEABLEOBJECT_H
