#ifndef CREATEABLEOBJECT_H
#define CREATEABLEOBJECT_H
namespace eiKernel {



class CreateableObject
{
public:
    static const unsigned char MAX_CLSID_LEN=255;
    char clsid[MAX_CLSID_LEN];
    unsigned char clsidLen;
    CreateableObject(){}
    CreateableObject(const char * clsid);
    unsigned char * serialize(unsigned char * msg);
    unsigned char * deserialize( unsigned char * msg);
};
}

#endif // CREATEABLEOBJECT_H
